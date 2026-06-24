#include <nanogui/texture.h>

#include <stb_image.h>

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>

NAMESPACE_BEGIN(nanogui)

struct Texture::UploadHandle::State {
    State(bool done) : done(done) { }

    std::atomic<uint32_t> ref_count { 1 };
    std::atomic<bool> done;
    std::mutex mutex;
    std::condition_variable cv;
};

Texture::UploadHandle::UploadHandle() = default;

Texture::UploadHandle::UploadHandle(State *state)
    : m_state(state) { }

Texture::UploadHandle::UploadHandle(UploadHandle &&other) noexcept
    : m_state(other.m_state) {
    other.m_state = nullptr;
}

Texture::UploadHandle &Texture::UploadHandle::operator=(UploadHandle &&other) noexcept {
    if (this != &other) {
        release();
        m_state = other.m_state;
        other.m_state = nullptr;
    }
    return *this;
}

Texture::UploadHandle::~UploadHandle() {
    release();
}

Texture::UploadHandle Texture::UploadHandle::pending() {
    return UploadHandle(new State(false));
}

Texture::UploadHandle Texture::UploadHandle::retain(const UploadHandle &handle) {
    State *state = handle.m_state;
    if (state)
        state->ref_count.fetch_add(1, std::memory_order_relaxed);
    return UploadHandle(state);
}

void Texture::UploadHandle::release() {
    State *state = m_state;
    m_state = nullptr;
    if (state && state->ref_count.fetch_sub(1, std::memory_order_acq_rel) == 1)
        delete state;
}

bool Texture::UploadHandle::is_done() const {
    State *state = m_state;
    return !state || state->done.load(std::memory_order_acquire);
}

void Texture::UploadHandle::wait() const {
    State *state = m_state;
    if (!state)
        return;

    std::unique_lock<std::mutex> lock(state->mutex);
    state->cv.wait(lock, [&] {
        return state->done.load(std::memory_order_acquire);
    });
}

void Texture::UploadHandle::mark_done() const {
    State *state = m_state;
    if (!state)
        return;

    {
        std::lock_guard<std::mutex> lock(state->mutex);
        state->done.store(true, std::memory_order_release);
    }
    state->cv.notify_all();
}

Texture::Texture(PixelFormat pixel_format,
                 ComponentFormat component_format,
                 const Vector2i &size,
                 InterpolationMode min_interpolation_mode,
                 InterpolationMode mag_interpolation_mode,
                 WrapMode wrap_mode,
                 uint8_t samples,
                 uint8_t flags,
                 bool mipmap_manual)
    : m_pixel_format(pixel_format),
      m_component_format(component_format),
      m_min_interpolation_mode(min_interpolation_mode),
      m_mag_interpolation_mode(mag_interpolation_mode),
      m_wrap_mode(wrap_mode),
      m_samples(samples),
      m_flags(flags),
      m_size(size),
      m_mipmap_manual(mipmap_manual) {

    if (m_flags & (uint8_t) TextureFlags::ShaderWrite) {
        if (m_samples > 1)
            throw std::runtime_error("Texture::Texture(): ShaderWrite is incompatible with MSAA (samples > 1)!");
        if (m_min_interpolation_mode == InterpolationMode::Trilinear ||
            m_mag_interpolation_mode == InterpolationMode::Trilinear)
            throw std::runtime_error("Texture::Texture(): ShaderWrite is incompatible with trilinear/mipmapped "
                                     "textures (write() only populates level 0)!");
    }

    init();
}

Texture::Texture(std::string_view filename,
                 InterpolationMode min_interpolation_mode,
                 InterpolationMode mag_interpolation_mode,
                 WrapMode wrap_mode)
    : m_component_format(ComponentFormat::UInt8),
      m_min_interpolation_mode(min_interpolation_mode),
      m_mag_interpolation_mode(mag_interpolation_mode),
      m_wrap_mode(wrap_mode),
      m_samples(1),
      m_flags(TextureFlags::ShaderRead),
      m_mipmap_manual(false) {
    int n = 0;
    using Holder = std::unique_ptr<uint8_t[], void(*)(void*)>;
    std::string filename_str(filename);
    Holder texture_data(stbi_load(filename_str.c_str(), &m_size.x(), &m_size.y(), &n, 0),
                        stbi_image_free);
    if (!texture_data)
        throw std::runtime_error("Could not load texture data from file \"" + filename_str + "\".");

    switch (n) {
        case 1: m_pixel_format = PixelFormat::R;    break;
        case 2: m_pixel_format = PixelFormat::RA;   break;
        case 3: m_pixel_format = PixelFormat::RGB;  break;
        case 4: m_pixel_format = PixelFormat::RGBA; break;
        default:
            throw std::runtime_error("Texture::Texture(): unsupported channel count!");
    }
    PixelFormat pixel_format = m_pixel_format;
    init();
    if (m_pixel_format != pixel_format)
        throw std::runtime_error("Texture::Texture(): pixel format not supported by the hardware!");
    upload((const uint8_t *) texture_data.get());
}

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
        case PixelFormat::BGR:          result = 3;  break;
        case PixelFormat::BGRA:         result = 4;  break;
        case PixelFormat::Depth:        result = 1;  break;
        case PixelFormat::DepthStencil: result = 2;  break;
        default: throw std::runtime_error("Texture::channels(): invalid "
                                          "pixel format!");
    }
    return result;
}

NAMESPACE_END(nanogui)
