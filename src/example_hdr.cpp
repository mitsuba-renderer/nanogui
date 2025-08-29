#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/combobox.h>
#include <nanogui/imageview.h>
#include <nanogui/texture.h>
#include <nanogui/chroma.h>
#include <algorithm>
#include <cmath>
#include <vector>

using namespace nanogui;
using namespace nanogui::ituth273;

struct PrimaryEntry {
    ColorPrimaries primary;
    const char* name;
};

static const PrimaryEntry primary_table[] = {
    { ColorPrimaries::BT2020,     "BT2020" },
    { ColorPrimaries::BT470BG,    "BT470BG" },
    { ColorPrimaries::BT470M,     "BT470M" },
    { ColorPrimaries::BT709,      "BT709" },
    { ColorPrimaries::Film,       "Film" },
    { ColorPrimaries::SMPTE170M,  "SMPTE170M" },
    { ColorPrimaries::SMPTE240M,  "SMPTE240M" },
    { ColorPrimaries::SMPTE428,   "SMPTE428" },
    { ColorPrimaries::SMPTE431,   "SMPTE431" },
    { ColorPrimaries::SMPTE432,   "SMPTE432" }
};
static const int num_primaries = sizeof(primary_table) / sizeof(primary_table[0]);

class HDRGamutTest : public Screen {
public:
    HDRGamutTest() : Screen(Vector2i(800, 600), "NanoGUI test", /* resizable */ true, /* maximized */ false,
                            /* fullscreen */ false, /* depth_buffer */ true, /* stencil_buffer */ false,
                            /* float_buffer */ true, /* gl_major */ 3, /* gl_minor */ 2) {
        inc_ref();

        Window *window = new Window(this, "HDR & Color gamut test");
        window->set_layout(new GroupLayout());

        m_texture = new Texture(
            Texture::PixelFormat::RGBA,
            Texture::ComponentFormat::Float32,
            Vector2i(512, 400),
            Texture::InterpolationMode::Nearest,
            Texture::InterpolationMode::Nearest
        );

        new Label(window, "Primaries");

        std::vector<std::string> primary_names;
        for (int i = 0; i < num_primaries; ++i) {
            primary_names.push_back(primary_table[i].name);
        }

        ColorPrimaries screen_primary = from_screen(this);
        int primary_index = 0;
        for (int i = 0; i < num_primaries; ++i) {
            if (primary_table[i].primary == screen_primary) {
                primary_index = i;
                break;
            }
        }

        ComboBox *primaries_cbox = new ComboBox(window, primary_names);
        primaries_cbox->set_selected_index(primary_index);

        primaries_cbox->set_callback([this](int index) {
            auto chroma_array = chroma(primary_table[index].primary);
            m_rec709_matrix = chroma_to_rec709_matrix(chroma_array);
            update_texture();
        });

        // Initialize with current selection
        auto chroma_array = chroma(primary_table[primary_index].primary);
        m_rec709_matrix = chroma_to_rec709_matrix(chroma_array);

        new Label(window, "Linear ramps (0..4), bars mark integer values. Top: LDR sRGB, bottom: HDR with chosen primaries.");

        ImageView *img = new ImageView(window);
        img->set_image(m_texture);
        img->set_size(Vector2i(512, 400));
        img->set_scale(pixel_ratio());

        perform_layout();
        window->set_position(Vector2i(5, 5));

        update_texture();
    }

private:
    float to_srgb(float value) {
        float sign = value < 0 ? -1.0f : 1.0f;
        value = std::abs(value);
        return sign * (value < 0.0031308f ? value * 12.92f 
                                           : 1.055f * std::pow(value, 1.0f/2.4f) - 0.055f);
    }

    void update_texture() {
        const int width = 512, height = 400;
        std::vector<float> img(width * height * 4, 0.0f);

        // Draw vertical grid lines at quarter positions
        for (int y = 0; y < height; ++y) {
            for (int x : {0, width/4, width/2, 3*width/4, width-1}) {
                float* p = &img[(y * width + x) * 4];
                p[0] = p[1] = p[2] = p[3] = 1.0f;
            }
        }

        // Draw color bars: red, green, blue, white
        const struct { int y0, y1; Vector3f srgb, primary; } bars[] = {
            {10,  90,  {1, 0, 0}, m_rec709_matrix[0]},
            {110, 190, {0, 1, 0}, m_rec709_matrix[1]},
            {210, 290, {0, 0, 1}, m_rec709_matrix[2]},
            {310, 390, {1, 1, 1}, {1, 1, 1}}
        };

        for (auto& bar : bars) {
            int mid_y = (bar.y0 + bar.y1) / 2;
            for (int y = bar.y0; y < bar.y1; ++y) {
                bool is_srgb = (y < mid_y);
                for (int x = 0; x < width; ++x) {
                    float ramp = x * 4.0f / (width - 1);
                    float* p = &img[(y * width + x) * 4];
                    for (int c = 0; c < 3; ++c) {
                        float val = ramp * (is_srgb ? bar.srgb[c] : bar.primary[c]);
                        p[c] = to_srgb(is_srgb ? std::min(val, 1.0f) : val);
                    }
                    p[3] = 1.0f;
                }
            }
        }

        m_texture->upload((uint8_t*)img.data());
    }

private:
    ref<Texture> m_texture;
    Matrix3f m_rec709_matrix;
};

int main(int /* argc */, char ** /* argv */) {
    nanogui::init();

    {
        ref<HDRGamutTest> app = new HDRGamutTest();
        app->dec_ref();
        app->draw_all();
        app->set_visible(true);
        nanogui::run(RunMode::Lazy);
    }

    nanogui::shutdown();
    return 0;
}
