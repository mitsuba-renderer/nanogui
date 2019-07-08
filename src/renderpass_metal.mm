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
                       uint8_t clear_stencil,
                       Object *blit_target)
    : m_targets(color_targets.size() + 2), m_clear(clear),
      m_clear_color(clear_color), m_clear_depth(clear_depth),
      m_clear_stencil(clear_stencil), m_viewport_offset(0),
      m_viewport_size(0), m_depth_test(DepthTest::Less),
      m_depth_write(true), m_cull_mode(CullMode::Back),
      m_blit_target(blit_target), m_active(false),
      m_command_buffer(nullptr), m_command_encoder(nullptr) {

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

    MTLRenderPassDescriptor *pass_descriptor =
        [MTLRenderPassDescriptor renderPassDescriptor];

    for (size_t i = 0; i < m_targets.size(); ++i) {
        Texture *texture = dynamic_cast<Texture *>(m_targets[i].get());
        Screen *screen   = dynamic_cast<Screen *>(m_targets[i].get());

        MTLRenderPassAttachmentDescriptor *att;
        if (i == 0)
            att = pass_descriptor.depthAttachment;
        else if (i == 1)
            att = pass_descriptor.stencilAttachment;
        else
            att = pass_descriptor.colorAttachments[i-2];

        att.loadAction = clear ? MTLLoadActionClear : MTLLoadActionLoad;
        att.storeAction = MTLStoreActionStore;

        if (texture) {
            if (!(texture->flags() & Texture::TextureFlags::RenderTarget))
                throw std::runtime_error("RenderPass::RenderPass(): target texture "
                                         "must be created with render_target=true!");
            m_viewport_size = max(m_viewport_size, texture->size());
        } else if (screen) {
            m_viewport_size = max(m_viewport_size, screen->framebuffer_size());
        } else if (m_targets[i].get()) {
            throw std::runtime_error("RenderPas::RenderPass(): invalid attachment type!");
        }

        if (i == 0) {
            ((MTLRenderPassDepthAttachmentDescriptor *) att).clearDepth = clear_depth;
        } else if (i == 1) {
            ((MTLRenderPassStencilAttachmentDescriptor *) att).clearStencil = clear_stencil;
        } else {
            MTLRenderPassColorAttachmentDescriptor *att2 =
                (MTLRenderPassColorAttachmentDescriptor *) att;
            if (clear_color.empty())
                att2.clearColor = MTLClearColorMake(0.f, 0.f, 0.f, 1.f);
            else
                att2.clearColor = MTLClearColorMake(clear_color[i].r(), clear_color[i].g(),
                                                    clear_color[i].b(), clear_color[i].w());
        }
    }

    m_pass_descriptor = (__bridge_retained void *) pass_descriptor;
}

RenderPass::~RenderPass() {
    (void) (__bridge_transfer MTLRenderPassDescriptor *) m_pass_descriptor;
}

void RenderPass::begin() {
#if !defined(NDEBUG)
    if (m_active)
        throw std::runtime_error("RenderPass::begin(): render pass is already active!");
#endif
    id<MTLCommandQueue> command_queue =
        (__bridge id<MTLCommandQueue>) metal_command_queue();

    id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];

    MTLRenderPassDescriptor *pass_descriptor =
        (__bridge MTLRenderPassDescriptor *) m_pass_descriptor;

    for (size_t i = 0; i < m_targets.size(); ++i) {
        Texture *texture = dynamic_cast<Texture *>(m_targets[i].get());
        Screen *screen = dynamic_cast<Screen *>(m_targets[i].get());
        id<MTLTexture> texture_handle = nil;

        if (texture) {
            texture_handle = (__bridge id<MTLTexture>) texture->texture_handle();
        } else if (screen) {
            if (i < 2) {
                Texture *screen_tex = screen->depth_stencil_texture();
                if (screen_tex && (i == 0 ||
                                  (i == 1 && (screen_tex->pixel_format() ==
                                              Texture::PixelFormat::DepthStencil)))) {
                    texture_handle = (__bridge id<MTLTexture>) screen_tex->texture_handle();
                }
            } else {
                id<CAMetalDrawable> drawable =
                    (__bridge id<CAMetalDrawable>) screen->metal_drawable();
                texture_handle = drawable.texture;
            }
        }

        MTLRenderPassAttachmentDescriptor *att;
        if (i == 0)
            att = pass_descriptor.depthAttachment;
        else if (i == 1)
            att = pass_descriptor.stencilAttachment;
        else
            att = pass_descriptor.colorAttachments[i-2];

        att.texture = texture_handle;

        RenderPass *blit_rp = dynamic_cast<RenderPass *>(m_blit_target.get());
        if (blit_rp && i < blit_rp->targets().size()) {
            const Texture *resolve_texture =
                dynamic_cast<Texture *>(blit_rp->targets()[i].get());

            id<MTLTexture> resolve_texture_handle =
                (__bridge id<MTLTexture>) resolve_texture->texture_handle();

            if (resolve_texture_handle.pixelFormat != texture_handle.pixelFormat)
                throw std::runtime_error("RenderPass::begin(): 'blit_target' pixel format mismatch!");
            else if (resolve_texture_handle.width  != texture_handle.width ||
                     resolve_texture_handle.height != texture_handle.height)
                throw std::runtime_error("RenderPass::begin(): 'blit_target' size mismatch!");
            att.storeAction = MTLStoreActionStoreAndMultisampleResolve;
            att.resolveTexture = resolve_texture_handle;
        }
    }

    Screen *blit_screen = dynamic_cast<Screen *>(m_blit_target.get());
    if (blit_screen) {
        id<CAMetalDrawable> drawable =
            (__bridge id<CAMetalDrawable>) blit_screen->metal_drawable();
        if (drawable.texture.pixelFormat != pass_descriptor.colorAttachments[0].texture.pixelFormat)
            throw std::runtime_error("RenderPass::begin(): 'blit_target' pixel format mismatch!");
        else if (drawable.texture.width  != pass_descriptor.colorAttachments[0].texture.width ||
                 drawable.texture.height != pass_descriptor.colorAttachments[0].texture.height)
            throw std::runtime_error("RenderPass::begin(): 'blit_target' pixel format mismatch!");
        pass_descriptor.colorAttachments[0].storeAction =
            MTLStoreActionStoreAndMultisampleResolve;
        pass_descriptor.colorAttachments[0].resolveTexture = drawable.texture;
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
