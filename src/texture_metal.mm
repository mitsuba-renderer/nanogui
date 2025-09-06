#include <nanogui/texture.h>
#include <nanogui/metal.h>
#import <Metal/Metal.h>

NAMESPACE_BEGIN(nanogui)

// Command queue for asynchronous cleanup operations
extern dispatch_queue_t cleanup_queue;

void Texture::init() {
    Vector2i size = m_size;
    m_size = 0;
    resize(size);

    MTLSamplerAddressMode wrap_mode_mtl;
    switch (m_wrap_mode) {
        case WrapMode::Repeat:       wrap_mode_mtl = MTLSamplerAddressModeRepeat; break;
        case WrapMode::ClampToEdge:  wrap_mode_mtl = MTLSamplerAddressModeClampToEdge; break;
        case WrapMode::MirrorRepeat: wrap_mode_mtl = MTLSamplerAddressModeMirrorRepeat; break;
        default: throw std::runtime_error("Texture::Texture(): invalid wrap mode!");
    }

    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    MTLSamplerDescriptor *sampler_desc = [MTLSamplerDescriptor new];

    sampler_desc.minFilter =
        m_min_interpolation_mode == InterpolationMode::Nearest
            ? MTLSamplerMinMagFilterNearest
            : MTLSamplerMinMagFilterLinear;

    sampler_desc.magFilter =
        m_mag_interpolation_mode == InterpolationMode::Nearest
            ? MTLSamplerMinMagFilterNearest
            : MTLSamplerMinMagFilterLinear;

    sampler_desc.mipFilter =
        m_min_interpolation_mode == InterpolationMode::Trilinear
            ? MTLSamplerMipFilterLinear
            : MTLSamplerMipFilterNotMipmapped;

    sampler_desc.sAddressMode = wrap_mode_mtl;
    sampler_desc.tAddressMode = wrap_mode_mtl;
    id<MTLSamplerState> sampler = [device newSamplerStateWithDescriptor:sampler_desc];

    m_sampler_state_handle = (__bridge_retained void *) sampler;
}

Texture::~Texture() {
    (void) (__bridge_transfer id<MTLTexture>) m_texture_handle;
    (void) (__bridge_transfer id<MTLSamplerState>) m_sampler_state_handle;
}

void Texture::upload_async(const uint8_t *data, void (*callback)(void*), void *payload) {
    if (!data)
        return;

    id<MTLTexture> texture = (__bridge id<MTLTexture>) m_texture_handle;
    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    id<MTLCommandQueue> command_queue = (__bridge id<MTLCommandQueue>) metal_command_queue();

    size_t data_size = bytes_per_pixel() * m_size.x() * m_size.y();
    size_t bytes_per_row = bytes_per_pixel() * m_size.x();

    id<MTLBuffer> buffer = [device newBufferWithBytesNoCopy: (void*)data
                                                     length: data_size
                                                    options: MTLResourceStorageModeShared
                                                deallocator: nil];

    if (buffer == nil) {
        // Likely, ``data`` is not page-aligned
        buffer = [device newBufferWithBytes: data
                                     length: data_size
                                    options: MTLResourceStorageModeShared];
    }

    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
    id<MTLBlitCommandEncoder> blit_encoder = [command_buffer blitCommandEncoder];

    [blit_encoder copyFromBuffer: buffer
                    sourceOffset: 0
               sourceBytesPerRow: bytes_per_row
             sourceBytesPerImage: data_size
                      sourceSize: MTLSizeMake(m_size.x(), m_size.y(), 1)
                       toTexture: texture
                destinationSlice: 0
                destinationLevel: 0
               destinationOrigin: MTLOriginMake(0, 0, 0)];

    [blit_encoder endEncoding];

    if (!m_mipmap_manual && m_min_interpolation_mode == InterpolationMode::Trilinear) {
        id<MTLBlitCommandEncoder> mipmap_encoder = [command_buffer blitCommandEncoder];
        [mipmap_encoder generateMipmapsForTexture:texture];
        [mipmap_encoder endEncoding];
    }

    if (callback) {
        [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> cb) {
            dispatch_async((__bridge dispatch_queue_t) metal_cleanup_queue(), ^{
                callback(payload);
            });
        }];
        [command_buffer commit];
    } else {
        // Synchronous path - wait for completion
        [command_buffer commit];
        [command_buffer waitUntilCompleted];
    }
}

void Texture::upload(const uint8_t *data) {
    upload_async(data, nullptr, nullptr);
}

void Texture::upload_sub_region(const uint8_t *data, const Vector2i& origin, const Vector2i& size) {
    id<MTLTexture> texture = (__bridge id<MTLTexture>) m_texture_handle;

    MTLTextureDescriptor *texture_desc =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat: texture.pixelFormat
                                                           width: (NSUInteger) size.x()
                                                          height: (NSUInteger) size.y()
                                                       mipmapped: NO];

    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    id<MTLCommandQueue> command_queue = (__bridge id<MTLCommandQueue>) metal_command_queue();
    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
    id<MTLBlitCommandEncoder> command_encoder = [command_buffer blitCommandEncoder];
    id<MTLTexture> temp_texture = [device newTextureWithDescriptor:texture_desc];

    [temp_texture replaceRegion: MTLRegionMake2D(0, 0, (NSUInteger) size.x(), (NSUInteger) size.y())
                  mipmapLevel: 0
                  withBytes: data
                  bytesPerRow: (NSUInteger) (bytes_per_pixel() * size.x())];

    [command_encoder
                 copyFromTexture: temp_texture
                     sourceSlice: 0
                     sourceLevel: 0
                    sourceOrigin: MTLOriginMake(0, 0, 0)
                      sourceSize: MTLSizeMake((NSUInteger) size.x(), (NSUInteger) size.y(), 1)
                       toTexture: texture
                destinationSlice: 0
                destinationLevel: 0
               destinationOrigin: MTLOriginMake((NSUInteger) origin.x(), (NSUInteger) origin.y(), 0)];

    [command_encoder endEncoding];
    [command_buffer commit];
    [command_buffer waitUntilCompleted];

    if (!m_mipmap_manual && m_min_interpolation_mode == InterpolationMode::Trilinear)
        generate_mipmap();
}

void Texture::download(uint8_t *data) {
    id<MTLCommandQueue> command_queue =
        (__bridge id<MTLCommandQueue>) metal_command_queue();
    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
    id<MTLBlitCommandEncoder> command_encoder =
        [command_buffer blitCommandEncoder];

    size_t row_bytes = bytes_per_pixel() * m_size.x(),
           img_bytes = row_bytes * m_size.y();

    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    id<MTLTexture> texture = (__bridge id<MTLTexture>) m_texture_handle;
    id<MTLBuffer> buffer =
        [device newBufferWithLength: img_bytes
                            options: MTLResourceStorageModeShared];

    [command_encoder
                 copyFromTexture: texture
                     sourceSlice: 0
                     sourceLevel: 0
                    sourceOrigin: MTLOriginMake(0, 0, 0)
                      sourceSize: MTLSizeMake(texture.width, texture.height, 1)
                        toBuffer: buffer
               destinationOffset: 0
          destinationBytesPerRow: row_bytes
        destinationBytesPerImage: img_bytes];

    [command_encoder endEncoding];
    [command_buffer commit];
    [command_buffer waitUntilCompleted];
    memcpy(data, buffer.contents, img_bytes);
}

void Texture::resize(const Vector2i &size) {
    if (m_size == size)
        return;
    m_size = size;
    if (m_texture_handle) {
        (void) (__bridge_transfer id<MTLTexture>) m_texture_handle;
        m_texture_handle = nullptr;
    }

    if (m_component_format == ComponentFormat::UInt32)
        m_component_format = ComponentFormat::UInt16;
    else if (m_component_format == ComponentFormat::Int32)
        m_component_format = ComponentFormat::Int16;

    if (m_pixel_format == PixelFormat::RGB)
        m_pixel_format = PixelFormat::RGBA;
    else if (m_pixel_format == PixelFormat::BGR)
        m_pixel_format = PixelFormat::BGRA;

    if (m_pixel_format == PixelFormat::BGRA &&
        m_component_format != ComponentFormat::UInt8)
        m_pixel_format = PixelFormat::RGBA;

    MTLPixelFormat pixel_format_mtl;
    switch (m_pixel_format) {
        case PixelFormat::R:
            switch (m_component_format) {
                case ComponentFormat::UInt8:   pixel_format_mtl = MTLPixelFormatR8Unorm;  break;
                case ComponentFormat::Int8:    pixel_format_mtl = MTLPixelFormatR8Snorm;  break;
                case ComponentFormat::UInt16:  pixel_format_mtl = MTLPixelFormatR16Unorm; break;
                case ComponentFormat::Int16:   pixel_format_mtl = MTLPixelFormatR16Snorm; break;
                case ComponentFormat::Float16: pixel_format_mtl = MTLPixelFormatR16Float; break;
                case ComponentFormat::Float32: pixel_format_mtl = MTLPixelFormatR32Float; break;
                default: throw std::runtime_error("Texture::Texture(): invalid component format!");
            }
            break;

        case PixelFormat::RA:
            switch (m_component_format) {
                case ComponentFormat::UInt8:   pixel_format_mtl = MTLPixelFormatRG8Unorm;  break;
                case ComponentFormat::Int8:    pixel_format_mtl = MTLPixelFormatRG8Snorm;  break;
                case ComponentFormat::UInt16:  pixel_format_mtl = MTLPixelFormatRG16Unorm; break;
                case ComponentFormat::Int16:   pixel_format_mtl = MTLPixelFormatRG16Snorm; break;
                case ComponentFormat::Float16: pixel_format_mtl = MTLPixelFormatRG16Float; break;
                case ComponentFormat::Float32: pixel_format_mtl = MTLPixelFormatRG32Float; break;
                default: throw std::runtime_error("Texture::Texture(): invalid component format!");
            }
            break;

        case PixelFormat::BGRA:
            switch (m_component_format) {
                case ComponentFormat::UInt8:   pixel_format_mtl = MTLPixelFormatBGRA8Unorm;  break;
                default: throw std::runtime_error("Texture::Texture(): invalid component format!");
            }
            break;

        case PixelFormat::RGBA:
            switch (m_component_format) {
                case ComponentFormat::UInt8:   pixel_format_mtl = MTLPixelFormatRGBA8Unorm;  break;
                case ComponentFormat::Int8:    pixel_format_mtl = MTLPixelFormatRGBA8Snorm;  break;
                case ComponentFormat::UInt16:  pixel_format_mtl = MTLPixelFormatRGBA16Unorm; break;
                case ComponentFormat::Int16:   pixel_format_mtl = MTLPixelFormatRGBA16Snorm; break;
                case ComponentFormat::Float16: pixel_format_mtl = MTLPixelFormatRGBA16Float; break;
                case ComponentFormat::Float32: pixel_format_mtl = MTLPixelFormatRGBA32Float; break;
                default: throw std::runtime_error("Texture::Texture(): invalid component format!");
            }
            break;

        case PixelFormat::Depth:
            switch (m_component_format) {
                case ComponentFormat::Int8:
                case ComponentFormat::UInt8:
                case ComponentFormat::Int16:
                case ComponentFormat::UInt16:
                    m_component_format = ComponentFormat::UInt16;
                    pixel_format_mtl = MTLPixelFormatDepth16Unorm;
                    break;

                case ComponentFormat::Int32:
                case ComponentFormat::UInt32:
                case ComponentFormat::Float16:
                case ComponentFormat::Float32:
                    m_component_format = ComponentFormat::Float32;
                    pixel_format_mtl = MTLPixelFormatDepth32Float;
                    break;

                default: throw std::runtime_error("Texture::Texture(): invalid component format!");
            }
            break;

        case PixelFormat::DepthStencil:
            switch (m_component_format) {
                case ComponentFormat::Int8:
                case ComponentFormat::UInt8:
                case ComponentFormat::Int16:
                case ComponentFormat::UInt16:
                case ComponentFormat::Int32:
                case ComponentFormat::UInt32:
                    m_component_format = ComponentFormat::UInt32;
                    pixel_format_mtl = MTLPixelFormatDepth24Unorm_Stencil8;
                    break;

                case ComponentFormat::Float16:
                case ComponentFormat::Float32:
                    m_component_format = ComponentFormat::Float32;
                    pixel_format_mtl = MTLPixelFormatDepth32Float_Stencil8;
                    break;

                default: throw std::runtime_error("Texture::Texture(): invalid component format!");
            }
            break;

        default:
            throw std::runtime_error("Texture::Texture(): invalid pixel format!");
    }

    bool mipmap = m_min_interpolation_mode == InterpolationMode::Trilinear;
    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    MTLTextureDescriptor *texture_desc =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat: pixel_format_mtl
                                                           width: (NSUInteger) m_size.x()
                                                          height: (NSUInteger) m_size.y()
                                                       mipmapped: mipmap];
    texture_desc.storageMode = MTLStorageModePrivate;
    texture_desc.usage = 0;

    if (m_samples > 1) {
        texture_desc.textureType = MTLTextureType2DMultisample;
        texture_desc.sampleCount = m_samples;
    }

    if (m_flags & (uint8_t) TextureFlags::ShaderRead)
        texture_desc.usage |= MTLTextureUsageShaderRead;
    if (m_flags & (uint8_t) TextureFlags::RenderTarget)
        texture_desc.usage |= MTLTextureUsageRenderTarget;
    if (texture_desc.usage == 0)
        throw std::runtime_error("Texture::Texture(): flags must either "
                                 "specify ShaderRead, RenderTarget, or both!");

    id<MTLTexture> texture = [device newTextureWithDescriptor:texture_desc];
    m_texture_handle = (__bridge_retained void *) texture;
}

void Texture::generate_mipmap() {
    id<MTLTexture> texture = (__bridge id<MTLTexture>) m_texture_handle;
    id<MTLCommandQueue> command_queue = (__bridge id<MTLCommandQueue>) metal_command_queue();
    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
    id<MTLBlitCommandEncoder> command_encoder = [command_buffer blitCommandEncoder];

    [command_encoder generateMipmapsForTexture: texture];
    [command_encoder endEncoding];
    [command_buffer commit];
}

NAMESPACE_END(nanogui)
