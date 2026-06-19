#include <nanogui/shader.h>
#include <nanogui/metal.h>
#include <nanogui/screen.h>
#include <nanogui/texture.h>
#include <nanogui/renderpass.h>
#include "shader_impl.h"

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#define NANOGUI_BUFFER_THRESHOLD 64

NAMESPACE_BEGIN(nanogui)

id<MTLFunction> compile_metal_shader(id<MTLDevice> device,
                                     std::string_view name,
                                     std::string_view type_str,
                                     std::string_view src) {
    if (src.empty())
        return nil;

    id<MTLLibrary> library = nil;
    NSError *error = nil;
    std::string activity;
    if (src.size() > 4 && strncmp(src.data(), "MTLB", 4) == 0) {
        dispatch_data_t data = dispatch_data_create(
            src.data(), src.size(), NULL, DISPATCH_DATA_DESTRUCTOR_DEFAULT);
        library = [device newLibraryWithData:data error:&error];
        activity = "load";
    } else {
        NSString *str = [[NSString alloc] initWithBytes:src.data()
                                                 length:src.size()
                                               encoding:NSUTF8StringEncoding];
        MTLCompileOptions *opts = [MTLCompileOptions new];
        library = [device newLibraryWithSource:str options:opts error:&error];
        activity = "compile";
    }
    if (error) {
        const char *error_shader = [[error description] UTF8String];
        throw std::runtime_error(
            std::string("compile_metal_shader(): unable to ") + activity + " " +
            std::string(type_str) + " shader \"" + std::string(name) + "\":\n\n" + error_shader);
    }

    NSArray<NSString *> *function_names = [library functionNames];
    if ([function_names count] != 1)
        throw std::runtime_error("compile_metal_shader(name=\"" + std::string(name) +
                                 "\"): library must contain exactly 1 shader!");
    NSString *function_name = [function_names objectAtIndex: 0];

    id<MTLFunction> function = [library newFunctionWithName: function_name];
    if (!function)
        throw std::runtime_error("compile_metal_shader(name=\"" + std::string(name) +
                                 "\"): function not found!");

    return function;
}

Shader::Shader(RenderPass *render_pass,
               std::string_view name,
               std::string_view vertex_shader,
               std::string_view fragment_shader,
               BlendMode blend_mode)
    : p(new Impl()) {
    p->render_pass = render_pass;
    p->name = name;
    p->blend_mode = blend_mode;
    p->pipeline_state = nullptr;
    id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
    id<MTLFunction> vertex_func   = compile_metal_shader(device, name, "vertex", vertex_shader),
                    fragment_func = compile_metal_shader(device, name, "fragment", fragment_shader);

    MTLRenderPipelineDescriptor *pipeline_desc =
        [MTLRenderPipelineDescriptor new];
    pipeline_desc.vertexFunction = vertex_func;
    pipeline_desc.fragmentFunction = fragment_func;

    std::vector<Object*> &targets = render_pass->targets();
    int sample_count = 1;

    for (size_t i = 0; i < targets.size(); ++i) {
        Texture *texture = dynamic_cast<Texture *>(targets[i]);
        Screen *screen = dynamic_cast<Screen *>(targets[i]);

        MTLPixelFormat pixel_format;
        if (targets[i] == nullptr) {
            continue;
        } else if (screen) {
            if (i == 0 || i == 1) {
                Texture *depth_stencil_texture = screen->depth_stencil_texture();
                if (!depth_stencil_texture ||
                    (i == 1 && depth_stencil_texture->pixel_format() !=
                                   Texture::PixelFormat::DepthStencil))
                    throw std::runtime_error("Shader::Shader(): Screen not configured for depth/stencil rendering");
                pixel_format = ((__bridge id<MTLTexture>) depth_stencil_texture->texture_handle()).pixelFormat;
            } else {
                pixel_format = ((__bridge CAMetalLayer *) screen->metal_layer()).pixelFormat;
            }
        } else if (texture) {
            pixel_format = ((__bridge id<MTLTexture>) texture->texture_handle()).pixelFormat;
            sample_count = std::max((int) texture->samples(), sample_count);
        } else {
            throw std::runtime_error(
                "Shader::Shader(): invalid target type! (either a Screen or "
                "Texture instance must be specified!)");
        }

        if (i == 0)
            pipeline_desc.depthAttachmentPixelFormat = pixel_format;
        else if (i == 1)
            pipeline_desc.stencilAttachmentPixelFormat = pixel_format;
        else
            pipeline_desc.colorAttachments[i-2].pixelFormat = pixel_format;

        if (i >= 2 && blend_mode == BlendMode::AlphaBlend) {
            MTLRenderPipelineColorAttachmentDescriptor *att =
                pipeline_desc.colorAttachments[i - 2];
            att.blendingEnabled             = YES;
            att.rgbBlendOperation           = MTLBlendOperationAdd;
            att.alphaBlendOperation         = MTLBlendOperationAdd;
            att.sourceRGBBlendFactor        = MTLBlendFactorSourceAlpha;
            att.sourceAlphaBlendFactor      = MTLBlendFactorSourceAlpha;
            att.destinationRGBBlendFactor   = MTLBlendFactorOneMinusSourceAlpha;
            att.destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        }
    }

    pipeline_desc.rasterSampleCount = sample_count;

    NSError *error = nil;
    MTLRenderPipelineReflection *reflection = nil;
    id<MTLRenderPipelineState> pipeline_state =
        [device newRenderPipelineStateWithDescriptor: pipeline_desc
                                             options: MTLPipelineOptionArgumentInfo
                                          reflection: &reflection
                                               error: &error];

    if (error) {
        const char *error_pipeline = [[error description] UTF8String];
        throw std::runtime_error(
            std::string("compile_metal_pipeline(): unable to create render pipeline state!\n\n") +
            error_pipeline);
    }

    p->pipeline_state = (__bridge_retained void *) pipeline_state;

    for (MTLArgument *arg in [reflection vertexArguments]) {
        std::string name = [arg.name UTF8String];
        if (p->buffers.find(name) != p->buffers.end())
            throw std::runtime_error(
                std::string("Shader::Shader(): \"") + std::string(name) +
                "\": duplicate argument name in shader code!");
        else if (name == "indices")
            throw std::runtime_error(
                "Shader::Shader(): argument name 'indices' is reserved!");

        Buffer &buf = p->buffers[std::string(name)];
        buf.index = arg.index;
        if (arg.type == MTLArgumentTypeBuffer)
            buf.type = VertexBuffer;
        else if (arg.type == MTLArgumentTypeTexture)
            buf.type = VertexTexture;
        else if (arg.type == MTLArgumentTypeSampler)
            buf.type = VertexSampler;
        else
            throw std::runtime_error(std::string("Shader::Shader(): \"") + std::string(name) +
                                     "\": unsupported argument type!");
    }

    for (MTLArgument *arg in [reflection fragmentArguments]) {
        std::string name = [arg.name UTF8String];
        if (p->buffers.find(name) != p->buffers.end())
            throw std::runtime_error(
                std::string("Shader::Shader(): \"") + std::string(name) +
                "\": duplicate argument name in shader code!");
        else if (name == "indices")
            throw std::runtime_error(
                "Shader::Shader(): argument name 'indices' is reserved!");

        Buffer &buf = p->buffers[std::string(name)];
        buf.index = arg.index;
        if (arg.type == MTLArgumentTypeBuffer)
            buf.type = FragmentBuffer;
        else if (arg.type == MTLArgumentTypeTexture)
            buf.type = FragmentTexture;
        else if (arg.type == MTLArgumentTypeSampler)
            buf.type = FragmentSampler;
        else
            throw std::runtime_error(std::string("Shader::Shader(): \"") + std::string(name) +
                                     "\": unsupported argument type!");
    }

    Buffer &buf = p->buffers["indices"];
    buf.index = -1;
    buf.type = IndexBuffer;
}

Shader::~Shader() {
    for (const auto &[key, buf] : p->buffers) {
        if (!buf.buffer)
            continue;
        if (buf.type == VertexBuffer ||
            buf.type == FragmentBuffer ||
            buf.type == IndexBuffer) {
            // Free as allocated: index buffers are always MTLBuffers
            if (buf.size <= NANOGUI_BUFFER_THRESHOLD && buf.type != IndexBuffer)
                delete[] (uint8_t *) buf.buffer;
            else
                (void) (__bridge_transfer id<MTLBuffer>) buf.buffer;
        } else if (buf.type == VertexTexture ||
                   buf.type == FragmentTexture) {
            (void) (__bridge_transfer id<MTLTexture>) buf.buffer;
        } else if (buf.type == VertexSampler ||
                   buf.type == FragmentSampler) {
            (void) (__bridge_transfer id<MTLSamplerState>) buf.buffer;
        } else {
            fprintf(stderr, "Shader::~Shader(): unknown buffer type!");
        }
    }
    (void) (__bridge_transfer id<MTLRenderPipelineState>) p->pipeline_state;
    delete p;
}

void Shader::set_buffer(std::string_view name,
                        VariableType dtype,
                        size_t ndim,
                        const size_t *shape,
                        const void *data) {
    auto it = p->buffers.find(name);
    if (it == p->buffers.end())
        throw std::runtime_error(
            std::string("Shader::set_buffer(): could not find argument named \"") + std::string(name) + "\"");
    Buffer &buf = it.value();
    if (!(buf.type == VertexBuffer ||
          buf.type == FragmentBuffer ||
          buf.type == IndexBuffer))
        throw std::runtime_error(
            std::string("Shader::set_buffer(): argument named \"") + std::string(name) + "\" is not a buffer!");

    for (size_t i = 0; i < 3; ++i)
        buf.shape[i] = i < ndim ? shape[i] : 1;

    size_t size = type_size(dtype) * buf.shape[0] * buf.shape[1] * buf.shape[2];
    bool inline_data = size <= NANOGUI_BUFFER_THRESHOLD && name != "indices";

    // Release the previous allocation. Inline data lives on the heap and is
    // reused in place when the size is unchanged; everything else is an MTLBuffer.
    bool prev_inline = buf.buffer && buf.size <= NANOGUI_BUFFER_THRESHOLD &&
                       buf.type != IndexBuffer;
    if (buf.buffer && !(inline_data && prev_inline && buf.size == size)) {
        if (prev_inline)
            delete[] (uint8_t *) buf.buffer;
        else
            (void) (__bridge_transfer id<MTLBuffer>) buf.buffer;
        buf.buffer = nullptr;
    }

    if (inline_data) {
        // Bound inline via setVertex/FragmentBytes; no GPU buffer needed.
        if (!buf.buffer)
            buf.buffer = new uint8_t[size];
        memcpy(buf.buffer, data, size);
    } else {
        // Dynamic data: allocate a fresh CPU/GPU-shared buffer and copy into it.
        // The command buffer that draws with it retains it until the GPU is
        // finished, so our reference can be released on the next update.
        id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
        buf.buffer = (__bridge_retained void *)
            [device newBufferWithBytes: data
                                length: size
                               options: MTLResourceStorageModeShared];
    }

    buf.dtype = dtype;
    buf.ndim  = ndim;
    buf.size  = size;
}

void Shader::set_texture(std::string_view name, Texture *texture) {
    auto it = p->buffers.find(name);
    if (it == p->buffers.end())
        throw std::runtime_error(
            std::string("Shader::set_texture(): could not find argument named \"") + std::string(name) + "\"");
    Buffer &buf = p->buffers[std::string(name)];
    if (!(buf.type == VertexTexture || buf.type == FragmentTexture))
        throw std::runtime_error(
            std::string("Shader::set_texture(): argument named \"") + std::string(name) + "\" is not a texture!");

    if (buf.buffer) {
        (void) (__bridge_transfer id<MTLTexture>) buf.buffer;
        buf.buffer = nullptr;
    }

    buf.buffer = (__bridge_retained void *) ((__bridge id<MTLTexture>)
                                                 texture->texture_handle());

    std::string sampler_name;
    if (name.length() > 8 && name.compare(name.length() - 8, 8, "_texture") == 0)
        sampler_name = std::string(name.substr(0, name.length()-8)) + "_sampler";
    else
        sampler_name = std::string(name) + "_sampler";

    if (p->buffers.find(sampler_name) != p->buffers.end()) {
        /* Also set the sampler state */
        Buffer &buf2 = p->buffers[sampler_name];

        if (buf2.buffer) {
            (void) (__bridge_transfer id<MTLTexture>) buf2.buffer;
            buf2.buffer = nullptr;
        }

        buf2.buffer =
            (__bridge_retained void *) ((__bridge id<MTLSamplerState>)
                                            texture->sampler_state_handle());
    }
}

void Shader::begin() {
    id<MTLRenderPipelineState> pipeline_state =
        (__bridge id<MTLRenderPipelineState>) p->pipeline_state;
    id<MTLRenderCommandEncoder> command_enc =
        (__bridge id<MTLRenderCommandEncoder>) p->render_pass->command_encoder();

    [command_enc setRenderPipelineState: pipeline_state];

    for (const auto &[key, buf] : p->buffers) {
        bool indices = buf.type == IndexBuffer;
        if (!buf.buffer) {
            if (!indices)
                fprintf(stderr,
                        "Shader::begin(): shader \"%s\" has an unbound "
                        "argument \"%s\"!\n",
                        p->name.c_str(), key.c_str());
            continue;
        }

        switch (buf.type) {
            case VertexTexture: {
                    id<MTLTexture> texture = (__bridge id<MTLTexture>) buf.buffer;
                    [command_enc setVertexTexture: texture atIndex: buf.index];
                }
                break;

            case FragmentTexture: {
                    id<MTLTexture> texture = (__bridge id<MTLTexture>) buf.buffer;
                    [command_enc setFragmentTexture: texture atIndex: buf.index];
                }
                break;

            case VertexSampler: {
                    id<MTLSamplerState> state = (__bridge id<MTLSamplerState>) buf.buffer;
                    [command_enc setVertexSamplerState: state atIndex: buf.index];
                }
                break;

            case FragmentSampler: {
                    id<MTLSamplerState> state = (__bridge id<MTLSamplerState>) buf.buffer;
                    [command_enc setFragmentSamplerState: state atIndex: buf.index];
                }
                break;

            default:
                if (buf.size <= NANOGUI_BUFFER_THRESHOLD && !indices) {
                    if (buf.type == VertexBuffer)
                        [command_enc setVertexBytes: buf.buffer
                                             length: buf.size
                                            atIndex: buf.index];
                    else if (buf.type == FragmentBuffer)
                        [command_enc setFragmentBytes: buf.buffer
                                               length: buf.size
                                              atIndex: buf.index];
                    else
                        throw std::runtime_error("Shader::begin(): unexpected buffer type!");
                } else {
                    id<MTLBuffer> buffer = (__bridge id<MTLBuffer>) buf.buffer;
                    if (buf.type == VertexBuffer)
                        [command_enc setVertexBuffer: buffer
                                              offset: 0
                                             atIndex: buf.index];
                    else if (buf.type == FragmentBuffer)
                        [command_enc setFragmentBuffer: buffer
                                                offset: 0
                                               atIndex: buf.index];
                }
                break;
        }
    }
}

void Shader::end() {
    /* No-op */
}

void Shader::draw_array(PrimitiveType primitive_type,
                        size_t offset, size_t count,
                        bool indexed) {
    MTLPrimitiveType primitive_type_mtl;
    switch (primitive_type) {
        case PrimitiveType::Point:         primitive_type_mtl = MTLPrimitiveTypePoint;         break;
        case PrimitiveType::Line:          primitive_type_mtl = MTLPrimitiveTypeLine;          break;
        case PrimitiveType::LineStrip:     primitive_type_mtl = MTLPrimitiveTypeLineStrip;     break;
        case PrimitiveType::Triangle:      primitive_type_mtl = MTLPrimitiveTypeTriangle;      break;
        case PrimitiveType::TriangleStrip: primitive_type_mtl = MTLPrimitiveTypeTriangleStrip; break;
        default: throw std::runtime_error("Shader::draw_array(): invalid primitive type!");
    }

    id<MTLRenderCommandEncoder> command_enc =
        (__bridge id<MTLRenderCommandEncoder>) p->render_pass->command_encoder();

    if (!indexed) {
        [command_enc drawPrimitives: primitive_type_mtl
                        vertexStart: offset
                        vertexCount: count];
    } else {
        id<MTLBuffer> index_buffer =
            (__bridge id<MTLBuffer>) p->buffers["indices"].buffer;
        [command_enc drawIndexedPrimitives: primitive_type_mtl
                                indexCount: count
                                 indexType: MTLIndexTypeUInt32
                               indexBuffer: index_buffer
                         indexBufferOffset: offset * 4];
    }
}

NAMESPACE_END(nanogui)
