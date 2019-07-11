#include <nanogui/texture.h>
#include <nanogui/opengl.h>
#include "opengl_check.h"

#if !defined(GL_HALF_FLOAT)
#  define GL_HALF_FLOAT 0x140B
#endif

#if !defined(GL_DEPTH_STENCIL)
#  define GL_DEPTH_STENCIL 0x84F9
#endif

NAMESPACE_BEGIN(nanogui)

static void gl_map_texture_format(Texture::PixelFormat &pixel_format,
                                  Texture::ComponentFormat &component_format,
                                  GLenum &pixel_format_gl,
                                  GLenum &component_format_gl,
                                  GLenum &internal_format_gl);

void Texture::init() {
    GLuint min_filter = 0, mag_filter = 0;
    switch (m_interpolation_mode) {
        case InterpolationMode::Nearest:
            min_filter = mag_filter = GL_NEAREST;
            break;

        case InterpolationMode::Bilinear:
            min_filter = mag_filter = GL_LINEAR;
            break;

        case InterpolationMode::Trilinear:
            min_filter = GL_LINEAR;
            mag_filter = GL_LINEAR_MIPMAP_LINEAR;
            break;

        default: throw std::runtime_error("Texture::Texture(): invalid interpolation mode!");
    }

    GLuint wrap_mode_gl = 0;
    switch (m_wrap_mode) {
        case WrapMode::Repeat:       wrap_mode_gl = GL_REPEAT; break;
        case WrapMode::ClampToEdge:  wrap_mode_gl = GL_CLAMP_TO_EDGE; break;
        case WrapMode::MirrorRepeat: wrap_mode_gl = GL_MIRRORED_REPEAT; break;
        default: throw std::runtime_error("Texture::Texture(): invalid wrap mode!");
    }

    GLenum pixel_format_gl,
           component_format_gl,
           internal_format_gl;

    gl_map_texture_format(m_pixel_format,
                          m_component_format,
                          pixel_format_gl,
                          component_format_gl,
                          internal_format_gl);

    (void) pixel_format_gl; (void) component_format_gl;

    if (m_flags & (uint8_t) TextureFlags::ShaderRead) {
        CHK(glGenTextures(1, &m_texture_handle));
        CHK(glBindTexture(GL_TEXTURE_2D, m_texture_handle));
        CHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));
        CHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
        CHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode_gl));
        CHK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode_gl));

        if (m_flags & (uint8_t) TextureFlags::RenderTarget)
            upload(nullptr);
    } else if (m_flags & (uint8_t) TextureFlags::RenderTarget) {
        CHK(glGenRenderbuffers(1, &m_renderbuffer_handle));
        CHK(glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_handle));
        CHK(glRenderbufferStorage(GL_RENDERBUFFER, internal_format_gl,
                              (GLsizei) m_size.x(), (GLsizei) m_size.y()));
    } else {
        throw std::runtime_error(
            "Texture::Texture(): flags must either specify ShaderRead, RenderTarget, or both!");
    }
}

Texture::~Texture() {
    CHK(glDeleteTextures(1, &m_texture_handle));
    CHK(glDeleteRenderbuffers(1, &m_renderbuffer_handle));
}

void Texture::upload(const uint8_t *data) {
    GLenum pixel_format_gl,
           component_format_gl,
           internal_format_gl;

    gl_map_texture_format(m_pixel_format,
                          m_component_format,
                          pixel_format_gl,
                          component_format_gl,
                          internal_format_gl);

    if (m_texture_handle != 0) {
        CHK(glBindTexture(GL_TEXTURE_2D, m_texture_handle));
        CHK(glTexImage2D(GL_TEXTURE_2D, 0, internal_format_gl,
                     (GLsizei) m_size.x(), (GLsizei) m_size.y(),
                     0, pixel_format_gl, component_format_gl,
                     data));

        if (m_interpolation_mode == InterpolationMode::Trilinear)
            CHK(glGenerateMipmap(GL_TEXTURE_2D));
    } else {
        CHK(glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer_handle));
        CHK(glRenderbufferStorage(GL_RENDERBUFFER, internal_format_gl,
                              (GLsizei) m_size.x(), (GLsizei) m_size.y()));
    }
}

void Texture::download(uint8_t *data) {
#if defined(NANOGUI_USE_GLES2)
    (void) data;
    throw std::runtime_error("Texture::download(): not supported on GLES 2!");
#else
    if (m_texture_handle == 0)
        throw std::runtime_error("Texture::download(): no texture handle!");

    GLenum pixel_format_gl,
           component_format_gl,
           internal_format_gl;

    gl_map_texture_format(m_pixel_format,
                          m_component_format,
                          pixel_format_gl,
                          component_format_gl,
                          internal_format_gl);

    (void) internal_format_gl;
    CHK(glBindTexture(GL_TEXTURE_2D, m_texture_handle));
    CHK(glGetTexImage(GL_TEXTURE_2D, 0, pixel_format_gl, component_format_gl, data));

    if (m_flags & (uint8_t) TextureFlags::RenderTarget) {
        size_t stride = bytes_per_pixel() * m_size.x();
        std::unique_ptr<uint8_t[]> temp(new uint8_t[stride]);

        uint8_t *low = (uint8_t *) data,
                *high = low + (m_size.y() - 1) * stride;

        for (; low < high; low += stride, high -= stride) {
            memcpy(temp.get(), low, stride);
            memcpy(low, high, stride);
            memcpy(high, temp.get(), stride);
        }
    }
#endif
}

void Texture::resize(const Vector2i &size) {
    if (m_size == size)
        return;
    m_size = size;
    upload(nullptr);
}

static void gl_map_texture_format(Texture::PixelFormat &pixel_format,
                                  Texture::ComponentFormat &component_format,
                                  GLenum &pixel_format_gl,
                                  GLenum &component_format_gl,
                                  GLenum &internal_format_gl) {
    using PixelFormat = Texture::PixelFormat;
    using ComponentFormat = Texture::ComponentFormat;

    pixel_format_gl = component_format_gl = internal_format_gl = 0;

#if defined(NANOGUI_USE_OPENGL)
    switch (pixel_format) {
        case PixelFormat::R:
            pixel_format_gl = GL_RED;

            switch (component_format) {
                case ComponentFormat::Int8:    internal_format_gl = GL_R8;        break;
                case ComponentFormat::UInt8:   internal_format_gl = GL_R8_SNORM;  break;
                case ComponentFormat::Int16:   internal_format_gl = GL_R16;       break;
                case ComponentFormat::UInt16:  internal_format_gl = GL_R16_SNORM; break;
                case ComponentFormat::Float16: internal_format_gl = GL_R16F;      break;
                case ComponentFormat::Float32: internal_format_gl = GL_R32F;      break;
                default:
                    break;
            }
            break;

        case PixelFormat::RA:
            pixel_format_gl = GL_RG;

            switch (component_format) {
                case ComponentFormat::Int8:    internal_format_gl = GL_RG8;        break;
                case ComponentFormat::UInt8:   internal_format_gl = GL_RG8_SNORM;  break;
                case ComponentFormat::Int16:   internal_format_gl = GL_RG16;       break;
                case ComponentFormat::UInt16:  internal_format_gl = GL_RG16_SNORM; break;
                case ComponentFormat::Float16: internal_format_gl = GL_RG16F;      break;
                case ComponentFormat::Float32: internal_format_gl = GL_RG32F;      break;
                default:
                    break;
            }
            break;

        case PixelFormat::RGB:
            pixel_format_gl = GL_RGB;

            switch (component_format) {
                case ComponentFormat::Int8:    internal_format_gl = GL_RGB8;        break;
                case ComponentFormat::UInt8:   internal_format_gl = GL_RGB8_SNORM;  break;
                case ComponentFormat::Int16:   internal_format_gl = GL_RGB16;       break;
                case ComponentFormat::UInt16:  internal_format_gl = GL_RGB16_SNORM; break;
                case ComponentFormat::Float16: internal_format_gl = GL_RGB16F;      break;
                case ComponentFormat::Float32: internal_format_gl = GL_RGB32F;      break;
                default:
                    break;
            }
            break;

        case PixelFormat::RGBA:
            pixel_format_gl = GL_RGBA;

            switch (component_format) {
                case ComponentFormat::Int8:    internal_format_gl = GL_RGBA8;        break;
                case ComponentFormat::UInt8:   internal_format_gl = GL_RGBA8_SNORM;  break;
                case ComponentFormat::Int16:   internal_format_gl = GL_RGBA16;       break;
                case ComponentFormat::UInt16:  internal_format_gl = GL_RGBA16_SNORM; break;
                case ComponentFormat::Float16: internal_format_gl = GL_RGBA16F;      break;
                case ComponentFormat::Float32: internal_format_gl = GL_RGBA32F;      break;
                default:
                    break;
            }
            break;

        case PixelFormat::Depth:
            pixel_format_gl = GL_DEPTH_COMPONENT;

            switch (component_format) {
                case ComponentFormat::Int8:
                case ComponentFormat::UInt8:
                case ComponentFormat::Int16:
                case ComponentFormat::UInt16:
                case ComponentFormat::Int32:
                case ComponentFormat::UInt32:
                    component_format = ComponentFormat::UInt32;
                    internal_format_gl = GL_DEPTH_COMPONENT16;
                    break;

                case ComponentFormat::Float16:
                case ComponentFormat::Float32:
                    component_format = ComponentFormat::Float32;
                    internal_format_gl = GL_DEPTH_COMPONENT32F;
                    break;

                default:
                    break;
            }
            break;

        case PixelFormat::DepthStencil:
            pixel_format_gl = GL_DEPTH_STENCIL;

            switch (component_format) {
                case ComponentFormat::Int8:
                case ComponentFormat::UInt8:
                case ComponentFormat::Int16:
                case ComponentFormat::UInt16:
                case ComponentFormat::Int32:
                case ComponentFormat::UInt32:
                    component_format = ComponentFormat::UInt32;
                    internal_format_gl = GL_DEPTH_COMPONENT24;
                    break;

                case ComponentFormat::Float16:
                case ComponentFormat::Float32:
                    component_format = ComponentFormat::Float32;
                    internal_format_gl = GL_DEPTH_COMPONENT32F;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
#else
    switch (pixel_format) {
        case PixelFormat::R:
            pixel_format_gl = internal_format_gl = GL_LUMINANCE;
            break;

        case PixelFormat::RA:
            pixel_format_gl = internal_format_gl = GL_LUMINANCE_ALPHA;
            break;

        case PixelFormat::RGB:
            pixel_format_gl = internal_format_gl = GL_RGB;
            break;

        case PixelFormat::RGBA:
            pixel_format_gl = internal_format_gl = GL_RGBA;
            break;

        case PixelFormat::Depth:
            pixel_format_gl = internal_format_gl = GL_DEPTH_COMPONENT;
            break;

        case PixelFormat::DepthStencil:
            pixel_format_gl = internal_format_gl = GL_DEPTH_STENCIL;
            break;

        default:
            break;
    }
#endif

    switch (component_format) {
        case ComponentFormat::Int8:    component_format_gl = GL_BYTE;           break;
        case ComponentFormat::UInt8:   component_format_gl = GL_UNSIGNED_BYTE;  break;
        case ComponentFormat::Int16:   component_format_gl = GL_SHORT;          break;
        case ComponentFormat::UInt16:  component_format_gl = GL_UNSIGNED_SHORT; break;
        case ComponentFormat::Float16: component_format_gl = GL_HALF_FLOAT;     break;
        case ComponentFormat::Float32: component_format_gl = GL_FLOAT;          break;
        default:
            break;
    }

    if (component_format_gl == 0)
        throw std::runtime_error("gl_map_texture_format(): invalid component format!");
    if (pixel_format_gl == 0)
        throw std::runtime_error("gl_map_texture_format(): invalid pixel format!");
    if (internal_format_gl == 0)
        throw std::runtime_error("gl_map_texture_format(): component format unsupported "
                                 "for the given pixel format!");
}


NAMESPACE_END(nanogui)
