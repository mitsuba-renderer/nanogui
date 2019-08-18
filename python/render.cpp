#ifdef NANOGUI_PYTHON

#include "python.h"
#include <enoki/transform.h>

using enoki::EnokiType;

static EnokiType dtype_to_enoki(const py::dtype &dtype) {
    switch (dtype.kind()) {
        case 'i':
            switch (dtype.itemsize()) {
                case 1: return EnokiType::Int8;
                case 2: return EnokiType::Int16;
                case 4: return EnokiType::Int32;
                case 8: return EnokiType::Int64;
            }

        case 'u':
            switch (dtype.itemsize()) {
                case 1: return EnokiType::UInt8;
                case 2: return EnokiType::UInt16;
                case 4: return EnokiType::UInt32;
                case 8: return EnokiType::UInt64;
            }

        case 'f':
            switch (dtype.itemsize()) {
                case 2: return EnokiType::Float16;
                case 4: return EnokiType::Float32;
                case 8: return EnokiType::Float64;
            }
    }

    return EnokiType::Invalid;
}

static void shader_set_buffer(Shader &shader, const std::string &name, py::array array) {
    if (array.ndim() > 3)
        throw py::type_error("Shader::set_buffer(): tensor rank must be < 3!");
    array = py::array::ensure(array, py::array::c_style);

    EnokiType dtype = dtype_to_enoki(array.dtype());

    if (dtype == EnokiType::Invalid)
        throw py::type_error("Shader::set_buffer(): unsupported array dtype!");

    std::array<size_t, 3> dim {
        array.ndim() > 0 ? (size_t) array.shape(0) : 1,
        array.ndim() > 1 ? (size_t) array.shape(1) : 1,
        array.ndim() > 2 ? (size_t) array.shape(2) : 1
    };

    shader.set_buffer(name, dtype, array.ndim(), dim, array.data());
}

static py::array texture_download(Texture &texture) {
    const char *dtype_name;
    switch (texture.component_format()) {
        case Texture::ComponentFormat::UInt8:   dtype_name = "u1"; break;
        case Texture::ComponentFormat::Int8:    dtype_name = "i1"; break;
        case Texture::ComponentFormat::UInt16:  dtype_name = "u2"; break;
        case Texture::ComponentFormat::Int16:   dtype_name = "i2"; break;
        case Texture::ComponentFormat::UInt32:  dtype_name = "u4"; break;
        case Texture::ComponentFormat::Int32:   dtype_name = "i4"; break;
        case Texture::ComponentFormat::Float16: dtype_name = "f2"; break;
        case Texture::ComponentFormat::Float32: dtype_name = "f4"; break;
    }

    py::array result(
        py::dtype(dtype_name),
        std::vector<ssize_t> { texture.size().y(), texture.size().x(),
                               (ssize_t) texture.channels() },
        std::vector<ssize_t> { }
    );

    texture.download((uint8_t *) result.mutable_data());
    return result;
}

static void texture_upload(Texture &texture, py::array array) {
    size_t n_channels = array.ndim() == 3 ? array.shape(2) : 1;
    EnokiType dtype         = dtype_to_enoki(array.dtype()),
              dtype_texture = (EnokiType) texture.component_format();

    if (array.ndim() != 2 && array.ndim() != 3)
        throw std::runtime_error("Texture::upload(): expected a 2 or 3-dimensional array!");
    else if (array.shape(0) != texture.size().y() ||
             array.shape(1) != texture.size().x())
        throw std::runtime_error("Texture::upload(): array size does not match the texture!");
    else if (n_channels != texture.channels())
        throw std::runtime_error(
            std::string("Texture::upload(): number of color channels in array (") +
            std::to_string(n_channels) + ") does not match the texture (" +
            std::to_string(texture.channels()) + ")!");
    else if (dtype != dtype_texture)
        throw std::runtime_error(
            std::string("Texture::upload(): dtype of array (") +
            enoki_type_name(dtype) + ") does not match the texture (" +
            enoki_type_name(dtype_texture) + ")!");

    texture.upload((const uint8_t *) array.data());
}

void register_render(py::module &m) {
    using PixelFormat       = Texture::PixelFormat;
    using ComponentFormat   = Texture::ComponentFormat;
    using InterpolationMode = Texture::InterpolationMode;
    using WrapMode          = Texture::WrapMode;
    using TextureFlags      = Texture::TextureFlags;
    using PrimitiveType     = Shader::PrimitiveType;
    using BlendMode         = Shader::BlendMode;
    using DepthTest         = RenderPass::DepthTest;
    using CullMode          = RenderPass::CullMode;

    auto texture = py::class_<Texture, Object, ref<Texture>>(m, "Texture", D(Texture));

    py::enum_<PixelFormat>(texture, "PixelFormat", D(Texture, PixelFormat))
        .value("R", PixelFormat::R, D(Texture, PixelFormat, R))
        .value("RA", PixelFormat::RA, D(Texture, PixelFormat, RA))
        .value("RGB", PixelFormat::RGB, D(Texture, PixelFormat, RGB))
        .value("RGBA", PixelFormat::RGBA, D(Texture, PixelFormat, RGBA))
        .value("BGR", PixelFormat::BGR, D(Texture, PixelFormat, BGR))
        .value("BGRA", PixelFormat::BGRA, D(Texture, PixelFormat, BGRA))
        .value("Depth", PixelFormat::Depth, D(Texture, PixelFormat, Depth))
        .value("DepthStencil", PixelFormat::DepthStencil, D(Texture, PixelFormat, DepthStencil));

    py::enum_<ComponentFormat>(texture, "ComponentFormat", D(Texture, ComponentFormat))
        .value("UInt8", ComponentFormat::UInt8, D(Texture, ComponentFormat, UInt8))
        .value("Int8", ComponentFormat::Int8, D(Texture, ComponentFormat, Int8))
        .value("UInt16", ComponentFormat::UInt16, D(Texture, ComponentFormat, UInt16))
        .value("Int16", ComponentFormat::Int16, D(Texture, ComponentFormat, Int16))
        .value("UInt32", ComponentFormat::UInt32, D(Texture, ComponentFormat, UInt32))
        .value("Int32", ComponentFormat::Int32, D(Texture, ComponentFormat, Int32))
        .value("Float16", ComponentFormat::Float16, D(Texture, ComponentFormat, Float16))
        .value("Float32", ComponentFormat::Float32, D(Texture, ComponentFormat, Float32));

    py::enum_<InterpolationMode>(texture, "InterpolationMode", D(Texture, InterpolationMode))
        .value("Nearest", InterpolationMode::Nearest, D(Texture, InterpolationMode, Nearest))
        .value("Bilinear", InterpolationMode::Bilinear, D(Texture, InterpolationMode, Bilinear))
        .value("Trilinear", InterpolationMode::Trilinear, D(Texture, InterpolationMode, Trilinear));

    py::enum_<WrapMode>(texture, "WrapMode", D(Texture, WrapMode))
        .value("ClampToEdge", WrapMode::ClampToEdge, D(Texture, WrapMode, ClampToEdge))
        .value("Repeat", WrapMode::Repeat, D(Texture, WrapMode, Repeat))
        .value("MirrorRepeat", WrapMode::MirrorRepeat, D(Texture, WrapMode, MirrorRepeat));

    py::enum_<TextureFlags>(texture, "TextureFlags", D(Texture, TextureFlags), py::arithmetic())
        .value("ShaderRead", TextureFlags::ShaderRead, D(Texture, TextureFlags, ShaderRead))
        .value("RenderTarget", TextureFlags::RenderTarget, D(Texture, TextureFlags, RenderTarget));

    texture
        .def(py::init<PixelFormat, ComponentFormat, const Vector2i &,
                      InterpolationMode, InterpolationMode, WrapMode, uint8_t, uint8_t>(),
             D(Texture, Texture), "pixel_format"_a, "component_format"_a, "size"_a,
             "min_interpolation_mode"_a = InterpolationMode::Bilinear,
             "mag_interpolation_mode"_a = InterpolationMode::Bilinear,
             "wrap_mode"_a = WrapMode::ClampToEdge, "samples"_a = 1,
             "flags"_a = (uint8_t) TextureFlags::ShaderRead)
        .def(py::init<const std::string &, InterpolationMode, InterpolationMode, WrapMode>(),
             D(Texture, Texture, 2), "filename"_a,
             "min_interpolation_mode"_a = InterpolationMode::Bilinear,
             "mag_interpolation_mode"_a = InterpolationMode::Bilinear,
             "wrap_mode"_a = WrapMode::ClampToEdge)
        .def("pixel_format", &Texture::pixel_format, D(Texture, pixel_format))
        .def("component_format", &Texture::component_format, D(Texture, component_format))
        .def("min_interpolation_mode", &Texture::min_interpolation_mode, D(Texture, min_interpolation_mode))
        .def("mag_interpolation_mode", &Texture::mag_interpolation_mode, D(Texture, mag_interpolation_mode))
        .def("wrap_mode", &Texture::wrap_mode, D(Texture, wrap_mode))
        .def("samples", &Texture::samples, D(Texture, samples))
        .def("flags", &Texture::flags, D(Texture, flags))
        .def("size", &Texture::size, D(Texture, size))
        .def("bytes_per_pixel", &Texture::bytes_per_pixel, D(Texture, bytes_per_pixel))
        .def("channels", &Texture::channels, D(Texture, channels))
        .def("download", &texture_download, D(Texture, download))
        .def("upload", &texture_upload, D(Texture, upload))
        .def("resize", &Texture::resize, D(Texture, resize))
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
        .def("texture_handle", &Texture::texture_handle)
        .def("renderbuffer_handle", &Texture::renderbuffer_handle)
#elif defined(NANOGUI_USE_METAL)
        .def("texture_handle", &Texture::texture_handle)
        .def("sampler_state_handle", &Texture::sampler_state_handle)
#endif
        ;

    auto shader = py::class_<Shader, Object, ref<Shader>>(m, "Shader", D(Shader));

    py::enum_<BlendMode>(shader, "BlendMode", D(Shader, BlendMode))
        .value("None", BlendMode::None, D(Shader, BlendMode, None))
        .value("AlphaBlend", BlendMode::AlphaBlend, D(Shader, BlendMode, AlphaBlend));

    shader
        .def(py::init<RenderPass *, const std::string &,
                      const std::string &, const std::string &, Shader::BlendMode>(),
             D(Shader, Shader), "render_pass"_a, "name"_a, "vertex_shader"_a,
             "fragment_shader"_a, "blend_mode"_a = BlendMode::None)
        .def("name", &Shader::name, D(Shader, name))
        .def("blend_mode", &Shader::blend_mode, D(Shader, blend_mode))
        .def("set_buffer", &shader_set_buffer, D(Shader, set_buffer))
        .def("set_texture", &Shader::set_texture, D(Shader, set_texture))
        .def("begin", &Shader::begin, D(Shader, begin))
        .def("end", &Shader::end, D(Shader, end))
        .def("__enter__", &Shader::begin)
        .def("__exit__", [](Shader &s, py::handle, py::handle, py::handle) { s.end(); })
        .def("draw_array", &Shader::draw_array, D(Shader, draw_array),
             "primitive_type"_a, "offset"_a, "count"_a, "indexed"_a = false)
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
        .def("shader_handle", &Shader::shader_handle)
#elif defined(NANOGUI_USE_METAL)
        .def("pipeline_state", &Shader::pipeline_state)
#endif
#if defined(NANOGUI_USE_OPENGL)
        .def("vertex_array_handle", &Shader::vertex_array_handle)
#endif
        ;

    py::enum_<PrimitiveType>(shader, "PrimitiveType", D(Shader, PrimitiveType))
        .value("Point", PrimitiveType::Point, D(Shader, PrimitiveType, Point))
        .value("Line", PrimitiveType::Line, D(Shader, PrimitiveType, Line))
        .value("LineStrip", PrimitiveType::LineStrip, D(Shader, PrimitiveType, LineStrip))
        .value("Triangle", PrimitiveType::Triangle, D(Shader, PrimitiveType, Triangle))
        .value("TriangleStrip", PrimitiveType::TriangleStrip, D(Shader, PrimitiveType, TriangleStrip));

    auto renderpass = py::class_<RenderPass, Object, ref<RenderPass>>(m, "RenderPass", D(RenderPass))
        .def(py::init<std::vector<Object *>, Object *, Object *, Object *, bool>(),
             D(RenderPass, RenderPass), "color_targets"_a, "depth_target"_a = nullptr,
             "stencil_target"_a = nullptr, "blit_target"_a = nullptr, "clear"_a = true)
        .def("set_clear_color", &RenderPass::set_clear_color, D(RenderPass, set_clear_color))
        .def("clear_color", &RenderPass::clear_color, D(RenderPass, clear_color))
        .def("set_clear_depth", &RenderPass::set_clear_depth, D(RenderPass, set_clear_depth))
        .def("clear_depth", &RenderPass::clear_depth, D(RenderPass, clear_depth))
        .def("set_clear_stencil", &RenderPass::set_clear_stencil, D(RenderPass, set_clear_stencil))
        .def("clear_stencil", &RenderPass::clear_stencil, D(RenderPass, clear_stencil))
        .def("set_viewport", &RenderPass::set_viewport, D(RenderPass, set_viewport), "offset"_a, "size"_a)
        .def("viewport", &RenderPass::viewport, D(RenderPass, viewport))
        .def("set_depth_test", &RenderPass::set_depth_test, D(RenderPass, set_depth_test), "depth_test"_a, "depth_write"_a)
        .def("depth_test", &RenderPass::depth_test, D(RenderPass, depth_test))
        .def("set_cull_mode", &RenderPass::set_cull_mode, D(RenderPass, set_cull_mode))
        .def("cull_mode", &RenderPass::cull_mode, D(RenderPass, cull_mode))
        .def("begin", &RenderPass::begin, D(RenderPass, begin))
        .def("end", &RenderPass::end, D(RenderPass, end))
        .def("resize", &RenderPass::resize, D(RenderPass, resize))
        .def("blit_to", &RenderPass::blit_to, D(RenderPass, blit_to),
             "src_offset"_a, "src_size"_a, "dst"_a, "dst_offset"_a)
        .def("__enter__", &RenderPass::begin)
        .def("__exit__", [](RenderPass &rp, py::handle, py::handle, py::handle) { rp.end(); })
#if defined(NANOGUI_USE_OPENGL) || defined(NANOGUI_USE_GLES)
        .def("framebuffer_handle", &RenderPass::framebuffer_handle)
#elif defined(NANOGUI_USE_METAL)
        .def("command_encoder", &RenderPass::command_encoder)
#endif
        ;

    py::enum_<CullMode>(renderpass, "CullMode", D(RenderPass, CullMode))
        .value("Disabled", CullMode::Disabled, D(RenderPass, CullMode, Disabled))
        .value("Front", CullMode::Front, D(RenderPass, CullMode, Front))
        .value("Back", CullMode::Back, D(RenderPass, CullMode, Back));

    py::enum_<DepthTest>(renderpass, "DepthTest", D(RenderPass, DepthTest))
        .value("Never", DepthTest::Never, D(RenderPass, DepthTest, Never))
        .value("Less", DepthTest::Less, D(RenderPass, DepthTest, Less))
        .value("Equal", DepthTest::Equal, D(RenderPass, DepthTest, Equal))
        .value("LessEqual", DepthTest::LessEqual, D(RenderPass, DepthTest, LessEqual))
        .value("Greater", DepthTest::Greater, D(RenderPass, DepthTest, Greater))
        .value("NotEqual", DepthTest::NotEqual, D(RenderPass, DepthTest, NotEqual))
        .value("GreaterEqual", DepthTest::GreaterEqual, D(RenderPass, DepthTest, GreaterEqual))
        .value("Always", DepthTest::Always, D(RenderPass, DepthTest, Always));

    m.def("translate", &enoki::translate<Matrix4f, Vector3f>, "v"_a);
    m.def("scale", &enoki::scale<Matrix4f, Vector3f>, "v"_a);
    m.def("rotate", &enoki::rotate<Matrix4f, Vector3f>, "axis"_a, "angle"_a);
    m.def("perspective", &enoki::perspective<Matrix4f>, "fov"_a, "near"_a, "far"_a,
          "aspect"_a = 1.f);
    m.def("frustum", &enoki::frustum<Matrix4f>, "left"_a, "right"_a,
          "bottom"_a, "top"_a, "near"_a, "far"_a);
    m.def("frustum", &enoki::frustum<Matrix4f>, "left"_a, "right"_a,
          "bottom"_a, "top"_a, "near"_a, "far"_a);
    m.def("ortho", &enoki::ortho<Matrix4f>, "left"_a, "right"_a,
          "bottom"_a, "top"_a, "near"_a, "far"_a);
    m.def("look_at", &enoki::look_at<Matrix4f, Vector3f, Vector3f>, "origin"_a,
          "target"_a, "up"_a);
}

#endif
