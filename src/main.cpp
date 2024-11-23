#include <image/image.h>

int main() {
    image2d image({800, 600}, srgb_color_encoding{});
    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            float u = (float(x) + 0.5f) / float(image.width());
            float v = (float(y) + 0.5f) / float(image.height());
            image.set_pixel({x, y}, std::span<const float, 3>({u, v, 0.f}));
        }
    }

    image.write_png("output.png");
    return 0;
}