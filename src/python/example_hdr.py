import nanogui as ng
import numpy as np

class HDRGamutTest(ng.Screen):
    def __init__(self):
        super(HDRGamutTest, self).__init__(size=(800, 600), caption='NanoGUI test', float_buffer=True)
        window = ng.Window(self, 'HDR & Color gamut test')
        window.set_layout(ng.GroupLayout())

        self.texture = ng.Texture(
            pixel_format=ng.Texture.PixelFormat.RGBA,
            component_format=ng.Texture.ComponentFormat.Float32,
            min_interpolation_mode=ng.Texture.InterpolationMode.Nearest,
            mag_interpolation_mode=ng.Texture.InterpolationMode.Nearest,
            size=[512, 400],
        )

        ng.Label(window, "Primaries")
        primaries = sorted(ng.ituth273.ColorPrimaries, key=lambda x: x.__name__)
        primary_index = primaries.index(ng.ituth273.from_screen(self))
        primaries_cbox = ng.ComboBox(window, [p.__name__ for p in primaries])
        primaries_cbox.set_selected_index(primary_index)

        def primary_cb(index):
            p = ng.ituth273.chroma(primaries[index])
            rec709_matrix = ng.ituth273.chroma_to_rec709_matrix(p)
            self.rec709_matrix = np.from_dlpack(rec709_matrix)
            self.update_texture()

        primaries_cbox.set_callback(primary_cb)
        primary_cb(primary_index)

        ng.Label(window, "Linear ramps (0..4), bars mark integer values. Top: LDR sRGB, bottom: HDR with chosen primaries.")
        img = ng.ImageView(window)
        img.set_image(self.texture)
        img.set_size((512, 400))
        img.set_scale(self.pixel_ratio())

        self.perform_layout()
        window.set_position((5, 5))

    def create_color_bar(self, y_start, y_end, primary_color, srgb_color, max_value=4):
        img = np.zeros((y_end - y_start, 512, 4), dtype=np.float32)
        mid_y = (y_end - y_start) // 2

        ramp = np.linspace(0, max_value, 512)
        for i in range(3):
            img[:mid_y, :, i] = np.minimum(ramp * srgb_color[i], 1)
            img[mid_y:, :, i] = ramp * primary_color[i]
        img[..., 3] = 1

        return img

    def to_srgb(self, value):
        sign = np.sign(value)
        value = abs(value)
        return np.where(value < 0.0031308,
                        value * 12.92,
                        1.055 * value**(1.0/2.4) - 0.055) * sign

    def update_texture(self):
        """Update texture with color bars using rec709_matrix primaries and sRGB comparison"""
        img = np.zeros((400, 512, 4), dtype=np.float32)

        # Vertical grid lines
        img[:, 0*512//4, :] = img[:, 1*512//4, :] = 1
        img[:, 2*512//4, :] = img[:, 3*512//4, :] = 1
        img[:, 511, :] = 1

        bars = [
            (10, 90,   (1, 0, 0), self.rec709_matrix[:, 0]),
            (110, 190, (0, 1, 0), self.rec709_matrix[:, 1]),
            (210, 290, (0, 0, 1), self.rec709_matrix[:, 2]),
            (310, 390, (1, 1, 1), (1, 1, 1)),
        ]

        for y_start, y_end, srgb, primary in bars:
            img[y_start:y_end, :, :] = self.create_color_bar(y_start, y_end, primary, srgb)

        self.texture.upload(self.to_srgb(img))

def main():
    ng.init()
    app = HDRGamutTest()
    app.set_visible(True)
    ng.run(ng.RunMode.Lazy)
    del app
    ng.shutdown()

if __name__ == '__main__':
    main()
