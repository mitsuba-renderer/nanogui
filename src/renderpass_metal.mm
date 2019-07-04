#include <nanogui/renderpass.h>
#include <nanogui/screen.h>
#include <nanogui/texture.h>
#include <nanogui/metal.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

NAMESPACE_BEGIN(nanogui)

RenderPass::RenderPass(std::vector<Object *> color_targets,
                       Object *depth_target,
                       Object *stencil_target,
                       bool clear,
                       std::vector<Color> clear_color,
                       float clear_depth,
                       uint8_t clear_stencil)
    : m_targets(color_targets.size() + 2), m_clear(clear),
      m_clear_color(clear_color), m_clear_depth(clear_depth),
      m_clear_stencil(clear_stencil), m_viewport_offset(0),
      m_viewport_size(0), m_depth_test(DepthTest::Less),
      m_depth_write(true), m_cull_mode(CullMode::Back),
      m_active(false), m_command_buffer(nullptr),
      m_command_encoder(nullptr) {

    m_targets[0] = depth_target;
    m_targets[1] = stencil_target;
    for (size_t i = 0; i < color_targets.size(); ++i)
        m_targets[i + 2] = color_targets[i];

    if (!clear_color.empty() && clear_color.size() != color_targets.size())
        throw std::runtime_error(
            "RenderPass::RenderPass(): 'clear_color' must either be empty or "
            "match 'color_targets' in size.");

    if (!m_targets[0].get()) {
        m_depth_write = false;
        m_depth_test = DepthTest::Always;
    }

    for (size_t i = 0; i < m_targets.size(); ++i) {
        const Texture *texture = dynamic_cast<const Texture *>(m_targets[i].get());
        const Screen *screen   = dynamic_cast<const Screen *>(m_targets[i].get());
        if (texture) {
            if (!(texture->flags() & Texture::TextureFlags::RenderTarget))
                throw std::runtime_error("RenderPass::RenderPass(): target texture "
                                         "must be created with render_target=true!");
            m_viewport_size = max(m_viewport_size, texture->size());
        } else if (screen) {
            m_viewport_size = max(m_viewport_size, screen->framebuffer_size());
        }
    }

    MTLRenderPassDescriptor *pass_descriptor =
        [MTLRenderPassDescriptor renderPassDescriptor];

    for (size_t i = 0; i < color_targets.size(); ++i) {
        MTLRenderPassColorAttachmentDescriptor *att =
            pass_descriptor.colorAttachments[i];
        if (clear_color.empty())
            att.clearColor = MTLClearColorMake(0.f, 0.f, 0.f, 1.f);
        else
            att.clearColor = MTLClearColorMake(clear_color[i].r(), clear_color[i].g(),
                                               clear_color[i].b(), clear_color[i].w());
        att.loadAction = clear ? MTLLoadActionClear : MTLLoadActionLoad;
        att.storeAction = MTLStoreActionStore;
    }

    if (dynamic_cast<Texture *>(depth_target)) {
        MTLRenderPassDepthAttachmentDescriptor *att =
            pass_descriptor.depthAttachment;
        att.clearDepth = clear_depth;
        att.loadAction = clear ? MTLLoadActionClear : MTLLoadActionLoad;
        att.storeAction = MTLStoreActionStore;
    } else if (depth_target) {
        throw std::runtime_error("RenderPass::RenderPass(): invalid depth target type! "
                                 "(must refer to NULL or a Texture instance!)");
    }

    if (dynamic_cast<Texture *>(stencil_target)) {
        MTLRenderPassStencilAttachmentDescriptor *att =
            pass_descriptor.stencilAttachment;
        att.clearStencil = clear_stencil;
        att.loadAction = clear ? MTLLoadActionClear : MTLLoadActionLoad;
        att.storeAction = MTLStoreActionStore;
    } else if (stencil_target) {
        throw std::runtime_error("RenderPass::RenderPass(): invalid stencil target type! "
                                 "(must refer to NULL or a Texture instance!)");
    }

    m_pass_descriptor = (__bridge_retained void *) pass_descriptor;
}

RenderPass::~RenderPass() {
    (void) (__bridge_transfer MTLRenderPassDescriptor *) m_pass_descriptor;
}

void RenderPass::begin() {
#if !defined(NDEBUG)
    if (m_active)
        throw std::runtime_error("RenderPass::end(): render pass is already active!");
#endif
    id<MTLCommandQueue> command_queue =
        (__bridge id<MTLCommandQueue>) metal_command_queue();

    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];

    MTLRenderPassDescriptor *pass_descriptor =
        (__bridge MTLRenderPassDescriptor *) m_pass_descriptor;

    for (size_t i = 0; i < m_targets.size(); ++i) {
        const Texture *texture = dynamic_cast<const Texture *>(m_targets[i].get());
        const Screen *screen = dynamic_cast<const Screen *>(m_targets[i].get());
        id<MTLTexture> texture_handle = nil;
        if (texture) {
            texture_handle = (__bridge id<MTLTexture>) texture->texture_handle();
        } else if (screen) {
            id<CAMetalDrawable> drawable =
                (__bridge id<CAMetalDrawable>) screen->metal_drawable();
            texture_handle = drawable.texture;
        }
        if (i == 0)
            pass_descriptor.depthAttachment.texture = texture_handle;
        else if (i == 1)
            pass_descriptor.stencilAttachment.texture = texture_handle;
        else
            pass_descriptor.colorAttachments[i-2].texture = texture_handle;
    }

    id<MTLRenderCommandEncoder> command_encoder =
        [command_buffer renderCommandEncoderWithDescriptor: pass_descriptor];

    [command_encoder setFrontFacingWinding: MTLWindingCounterClockwise];

    m_command_buffer = (__bridge_retained void *) command_buffer;
    m_command_encoder = (__bridge_retained void *) command_encoder;
    m_active = true;

    set_viewport(m_viewport_offset, m_viewport_size);
    set_depth_test(m_depth_test, m_depth_write);
    set_cull_mode(m_cull_mode);
}

void RenderPass::end() {
#if !defined(NDEBUG)
    if (!m_active)
        throw std::runtime_error("RenderPass::end(): render pass is not active!");
#endif
    id<MTLCommandBuffer> command_buffer =
        (__bridge_transfer id<MTLCommandBuffer>) m_command_buffer;
    id<MTLRenderCommandEncoder> command_encoder =
        (__bridge_transfer id<MTLRenderCommandEncoder>) m_command_encoder;
    [command_encoder endEncoding];
    [command_buffer commit];
    m_command_encoder = nullptr;
    m_command_buffer = nullptr;
    m_active = false;
}

void RenderPass::resize(const Vector2i &size) {
    for (size_t i = 0; i < m_targets.size(); ++i) {
        Texture *texture = dynamic_cast<Texture *>(m_targets[i].get());
        if (texture)
            texture->resize(size);
    }
}

void RenderPass::set_viewport(const Vector2i &offset, const Vector2i &size) {
    m_viewport_offset = offset;
    m_viewport_size = size;
    if (m_active) {
        id<MTLRenderCommandEncoder> command_encoder =
            (__bridge id<MTLRenderCommandEncoder>) m_command_encoder;
        [command_encoder setViewport: (MTLViewport) {
          (double) offset.x(), (double) offset.y(),
          (double) size.x(),   (double) size.y(),
          0.0, 1.0 }
        ];
    }
}

void RenderPass::set_depth_test(DepthTest depth_test, bool depth_write) {
    m_depth_test = depth_test;
    m_depth_write = depth_write;
    if (m_active) {
        MTLDepthStencilDescriptor *depth_desc = [MTLDepthStencilDescriptor new];
        if (m_targets[0]) {
            MTLCompareFunction func;
            switch (depth_test) {
                case DepthTest::Never:        func = MTLCompareFunctionNever; break;
                case DepthTest::Less:         func = MTLCompareFunctionLess; break;
                case DepthTest::Equal:        func = MTLCompareFunctionEqual; break;
                case DepthTest::LessEqual:    func = MTLCompareFunctionLessEqual; break;
                case DepthTest::Greater:      func = MTLCompareFunctionGreater; break;
                case DepthTest::NotEqual:     func = MTLCompareFunctionNotEqual; break;
                case DepthTest::GreaterEqual: func = MTLCompareFunctionGreater; break;
                case DepthTest::Always:       func = MTLCompareFunctionAlways; break;
                default:
                    throw std::runtime_error(
                        "Shader::set_depth_test(): invalid depth test mode!");
            }
            depth_desc.depthCompareFunction = func;
            depth_desc.depthWriteEnabled = depth_write;
        } else {
            depth_desc.depthCompareFunction = MTLCompareFunctionAlways;
            depth_desc.depthWriteEnabled = NO;
        }

        id<MTLDevice> device = (__bridge id<MTLDevice>) metal_device();
        id<MTLDepthStencilState> depth_state =
            [device newDepthStencilStateWithDescriptor: depth_desc];
        id<MTLRenderCommandEncoder> command_encoder =
            (__bridge id<MTLRenderCommandEncoder>) m_command_encoder;
        [command_encoder setDepthStencilState: depth_state];
    }
}

void RenderPass::set_cull_mode(CullMode cull_mode) {
    m_cull_mode = cull_mode;
    if (m_active) {
        MTLCullMode cull_mode_mtl;
        switch (cull_mode) {
            case CullMode::Front:    cull_mode_mtl = MTLCullModeFront; break;
            case CullMode::Back:     cull_mode_mtl = MTLCullModeBack; break;
            case CullMode::Disabled: cull_mode_mtl = MTLCullModeNone; break;
            default: throw std::runtime_error("Shader::set_cull_mode(): invalid cull mode!");
        }
        id<MTLRenderCommandEncoder> command_encoder =
            (__bridge id<MTLRenderCommandEncoder>) m_command_encoder;
        [command_encoder setCullMode: cull_mode_mtl];
    }
}

NAMESPACE_END(nanogui)
