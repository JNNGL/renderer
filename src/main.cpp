#include <image/image.h>
#include <camera/perspective.h>

vec3f compute_pixel_color(std::shared_ptr<camera> camera, vec2i pixel) {
    std::optional<ray> cameraRay = camera->generate_ray({vec2f(pixel.x, pixel.y)});
    if (!cameraRay) return {};

    return cameraRay->direction() * 0.5f + vec3f(0.5f);
}

int main() {
    image2d image({800, 600}, srgb_color_encoding{});

    std::shared_ptr<camera> camera = std::make_shared<perspective_camera>(image.dimensions(), 45.f, transform{});

    for (int x = 0; x < image.width(); x++) {
        for (int y = 0; y < image.height(); y++) {
            vec3f color = compute_pixel_color(camera, {x, y});
            image.set_pixel({x, y}, std::span<const float, 3>({color.x, color.y, color.z}));
        }
    }

    image.write_png("output.png");
    return 0;
}