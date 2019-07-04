#include <nanogui/texture.h>
#include <nanogui/metal.h>
#import <Metal/Metal.h>

NAMESPACE_BEGIN(nanogui)

Texture::Texture(PixelFormat pixel_format,
                 ComponentFormat component_format,
                 const Vector2i &size,
                 InterpolationMode interpolation_mode,
                 WrapMode wrap_mode,
                 uint8_t flags)
    : m_pixel_format(pixel_format),
      m_component_format(component_format),
      m_interpolation_mode(interpolation_mode),
      m_wrap_mode(wrap_mode),
      m_flags(flags),
      m_size(size),
      m_texture_handle(nullptr),
      m_sampler_state_handle(nullptr) {

    resize(size);

    MTLSamplerMinMagFilter interpolation_mode_mtl;
    MTLSamplerMipFilter interpolation_mode_mip_mtl;

    switch (m_interpolation_mode) {
        case InterpolationMode::Nearest:
            interpolation_mode_mtl     = MTLSamplerMinMagFilterNearest;
            interpolation_mode_mip_mtl = MTLSamplerMipFilterNotMipmapped;
            break;

        case InterpolationMode::Bilinear:
            interpolation_mode_mtl     = MTLSamplerMinMagFilterLinear;
            interpolation_mode_mip_mtl = MTLSamplerMipFilterNotMipmapped;
            break;

        case InterpolationMode::Trilinear:
            interpolation_mode_mtl     = MTLSamplerMinMagFilterLinear;
            interpolation_mode_mip_mtl = MTLSamplerMipFilterLinear;
            break;

        default: throw std::runtime_error("Texture::Texture(): invalid interpolation mode!");
    }

    MTLSamplerAddressMode wrap_mode_mtl;
    switch (m_wrap_mode) {
        case WrapMode::Repeat:       wrap_mode_mtl = MTLSamplerAddressModeRepeat; break;
        case WrapMode::ClampToEdge:  wrap_mode_mtl = MTLSamplerAddressModeClampToEdge; break;
        case WrapMode::MirrorRepeat: wrap_mode_mtl = MTLSamplerAddressModeMirrorRepeat; break;
        default: throw std::runtime_error("Texture::Texture(): invalid wrap mode!");
    }

    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    MTLSamplerDescriptor *sampler_desc = [MTLSamplerDescriptor new];
    sampler_desc.minFilter = interpolation_mode_mtl;
    sampler_desc.magFilter = interpolation_mode_mtl;
    sampler_desc.mipFilter = interpolation_mode_mip_mtl;
    sampler_desc.sAddressMode = wrap_mode_mtl;
    sampler_desc.tAddressMode = wrap_mode_mtl;
    id<MTLSamplerState> sampler = [device newSamplerStateWithDescriptor:sampler_desc];

    m_sampler_state_handle = (__bridge_retained void *) sampler;
}

Texture::~Texture() {
    (void) (__bridge_transfer id<MTLTexture>) m_texture_handle;
    (void) (__bridge_transfer id<MTLSamplerState>) m_sampler_state_handle;
}

void Texture::upload(const uint8_t *data) {
    id<MTLTexture> texture = (__bridge id<MTLTexture>) m_texture_handle;

    [texture replaceRegion: MTLRegionMake2D(0, 0, (NSUInteger) m_size.x(), (NSUInteger) m_size.y())
             mipmapLevel: 0
             withBytes: data
             bytesPerRow: (NSUInteger) (bytes_per_pixel() * m_size.x())];

    if (m_interpolation_mode == InterpolationMode::Trilinear) {
        id<MTLCommandQueue> command_queue = (__bridge id<MTLCommandQueue>) metal_command_queue();
        id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];
        id<MTLBlitCommandEncoder> command_encoder = [command_buffer blitCommandEncoder];
        [command_encoder generateMipmapsForTexture: texture];
        [command_encoder endEncoding];
        [command_buffer commit];
        [command_buffer waitUntilCompleted];
    }
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
    if (m_texture_handle) {
        (void) (__bridge_transfer id<MTLTexture>) m_texture_handle;
        m_texture_handle = nullptr;
    }
    m_size = size;

    if (m_component_format == ComponentFormat::UInt32)
        m_component_format = ComponentFormat::UInt16;
    else if (m_component_format == ComponentFormat::Int32)
        m_component_format = ComponentFormat::Int16;

    if (m_pixel_format == PixelFormat::RGB)
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
                case ComponentFormat::UInt8:
                case ComponentFormat::Int8:
                case ComponentFormat::Int16:
                    m_component_format = ComponentFormat::UInt16;
                    pixel_format_mtl = MTLPixelFormatDepth16Unorm;
                    break;

                case ComponentFormat::Float16:
                    m_component_format = ComponentFormat::Float32;
                    pixel_format_mtl = MTLPixelFormatDepth32Float;
                    break;

                case ComponentFormat::UInt16:  pixel_format_mtl = MTLPixelFormatDepth16Unorm; break;
                case ComponentFormat::Float32: pixel_format_mtl = MTLPixelFormatDepth32Float; break;
                default: throw std::runtime_error("Texture::Texture(): invalid component format!");
            }
            break;

        case PixelFormat::DepthStencil:
            switch (m_component_format) {
                case ComponentFormat::UInt8:
                case ComponentFormat::Int8:
                case ComponentFormat::Int16:
                    m_component_format = ComponentFormat::UInt16;
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

    bool mipmap = m_interpolation_mode == InterpolationMode::Trilinear;
    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    MTLTextureDescriptor *texture_desc =
        [MTLTextureDescriptor texture2DDescriptorWithPixelFormat: pixel_format_mtl
                                                           width: (NSUInteger) m_size.x()
                                                          height: (NSUInteger) m_size.y()
                                                       mipmapped: mipmap];
    texture_desc.storageMode = MTLStorageModePrivate;
    texture_desc.usage = 0;

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

NAMESPACE_END(nanogui)
