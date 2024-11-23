#include "image.h"

#include <stb_image_write.h>
#include <cstring>

template <int channels, typename T>
void image2d<channels, T>::set_channel_raw(vec2i p, int c, T t) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return;
    contents[(p.y * resolution.x + p.x) * channels + c] = t;
}

template <int channels, typename T>
void image2d<channels, T>::set_channel(vec2i p, int c, float t) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return;
    set_channel_raw(p, c, encode_channel<T>(t));
}

template <int channels, typename T>
T image2d<channels, T>::get_channel_raw(vec2i p, int c) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return zero;
    return contents[(p.y * resolution.x + p.x) * channels + c];
}

template <int channels, typename T>
float image2d<channels, T>::get_channel(vec2i p, int c) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return 0.f;
    return decode_channel(get_channel_raw(p, c));
}

template <int channels, typename T>
void image2d<channels, T>::set_pixel_raw(vec2i p, std::span<const T, channels> t) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return;
    for (int i = 0; i < channels; i++) {
        set_channel_raw(p, i, t[i]);
    }
}

template <int channels, typename T>
void image2d<channels, T>::set_pixel(vec2i p, std::span<const float, channels> t) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return;
    for (int i = 0; i < channels; i++) {
        set_channel(p, i, t[i]);
    }
}

template<int channels, typename T>
std::array<T, channels> image2d<channels, T>::get_pixel_raw(vec2i p) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return pixel_zero_internal;
    std::array<T, channels> pixel = fill_pixel(zero);
    for (int i = 0; i < channels; i++) {
        pixel[i] = get_channel_raw(p, i);
    }
    return pixel;
}

template<int channels, typename T>
std::array<float, channels> image2d<channels, T>::get_pixel(vec2i p) {
    if (p.x < 0 || p.y < 0 || p.x >= resolution.x || p.y >= resolution.y) return pixel_zero;
    std::array<float, channels> pixel = fill_pixel(0.f);
    for (int i = 0; i < channels; i++) {
        pixel[i] = get_channel(p, i);
    }
    return pixel;
}

template <int channels, typename T>
void image2d<channels, T>::write_png(const std::string& filename) {
    auto* data = new uint8_t[resolution.x * resolution.y * channels];
    for (int x = 0; x < resolution.x; x++) {
        for (int y = 0; y < resolution.y; y++) {
            std::array<float, channels> in = get_pixel({x, y});
            std::array<uint8_t, channels> out{};
            encoding->from_linear(in, out);
            std::memcpy(&data[(y * resolution.x + x) * channels], out.data(), channels);
        }
    }

    stbi_flip_vertically_on_write(true);
    stbi_write_png(filename.c_str(), resolution.x, resolution.y, channels, data, 0);

    delete[] data;
}

template class image2d<1, float>;
template class image2d<2, float>;
template class image2d<3, float>;
template class image2d<4, float>;