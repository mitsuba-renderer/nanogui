#include <nanogui/texture.h>

NAMESPACE_BEGIN(nanogui)

size_t Texture::bytes_per_pixel() const {
    size_t result = 0;
    switch (m_component_format) {
        case ComponentFormat::UInt8:   result = 1; break;
        case ComponentFormat::Int8:    result = 1; break;
        case ComponentFormat::UInt16:  result = 2; break;
        case ComponentFormat::Int16:   result = 2; break;
        case ComponentFormat::UInt32:  result = 4; break;
        case ComponentFormat::Int32:   result = 4; break;
        case ComponentFormat::Float16: result = 2; break;
        case ComponentFormat::Float32: result = 4; break;
        default: throw std::runtime_error("Texture::bytes_per_pixel(): "
                                          "invalid component format!");
    }

    return result * channels();
}

size_t Texture::channels() const {
    size_t result = 1;
    switch (m_pixel_format) {
        case PixelFormat::R:            result = 1;  break;
        case PixelFormat::RA:           result = 2;  break;
        case PixelFormat::RGB:          result = 3;  break;
        case PixelFormat::RGBA:         result = 4;  break;
        case PixelFormat::Depth:        result = 1;  break;
        case PixelFormat::DepthStencil: result = 2;  break;
        default: throw std::runtime_error("Texture::channels(): invalid "
                                          "pixel format!");
    }
    return result;
}

NAMESPACE_END(nanogui)
