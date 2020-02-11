#include <nanogui/shader.h>

NAMESPACE_BEGIN(nanogui)

std::string Shader::Buffer::to_string() const {
    std::string result = "Buffer[type=";
    switch (type) {
        case BufferType::VertexBuffer: result += "vertex"; break;
        case BufferType::FragmentBuffer: result += "fragment"; break;
        case BufferType::UniformBuffer: result += "uniform"; break;
        case BufferType::IndexBuffer: result += "index"; break;
        default: result += "unknown"; break;
    }
    result += ", dtype=";
    result += type_name(dtype);
    result += ", shape=[";
    for (size_t i = 0; i < ndim; ++i) {
        result += std::to_string(shape[i]);
        if (i + 1 < ndim)
            result += ", ";
    }
    result += "]]";
    return result;
}

NAMESPACE_END(nanogui)
