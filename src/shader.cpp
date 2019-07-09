#include <nanogui/shader.h>

NAMESPACE_BEGIN(nanogui)

size_t enoki_type_size(enoki::EnokiType type) {
    using enoki::EnokiType;

    switch (type) {
        case EnokiType::UInt8:
        case EnokiType::Int8:
        case EnokiType::Bool:
            return 1;

        case EnokiType::UInt16:
        case EnokiType::Int16:
        case EnokiType::Float16:
            return 2;

        case EnokiType::UInt32:
        case EnokiType::Int32:
        case EnokiType::Float32:
            return 4;

        case EnokiType::UInt64:
        case EnokiType::Int64:
        case EnokiType::Float64:
            return 8;

        default:
            throw std::runtime_error("Unknown type!");
    }
}

const char *enoki_type_name(enoki::EnokiType type) {
    using enoki::EnokiType;

    switch (type) {
        case EnokiType::Bool:    return "bool";
        case EnokiType::UInt8:   return "uint8";
        case EnokiType::Int8:    return "int8";
        case EnokiType::UInt16:  return "uint16";
        case EnokiType::Int16:   return "int16";
        case EnokiType::UInt32:  return "uint32";
        case EnokiType::Int32:   return "int32";
        case EnokiType::UInt64:  return "uint64";
        case EnokiType::Int64:   return "int64";
        case EnokiType::Float16: return "float16";
        case EnokiType::Float32: return "float32";
        case EnokiType::Float64: return "float64";
        default:
            throw std::runtime_error("Unknown type!");
    }
}

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
    result += enoki_type_name(dtype);
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
