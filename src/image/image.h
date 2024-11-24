#pragma once

#include <math/vec.h>
#include <image/encoding.h>

#include <utility>
#include <string>
#include <memory>

template <typename T>
constexpr T encode_channel(float v);
template <typename T>
constexpr float decode_channel(T t);

template <>
inline constexpr float encode_channel(float v) { return v; }
template <>
inline constexpr float decode_channel(float t) { return t; }

template <int channels = 3, typename T = float>
class image2d {
public:
    image2d() = default;
    template <typename E = color_encoding>
    image2d(vec2i res, E encoding = {})
        : resolution(res), encoding(std::make_unique<E>(encoding)) {
        contents = new T[res.x * res.y * channels];
    }
    ~image2d() {
        delete[] contents;
    }

    void set_channel_raw(vec2i p, int c, T t);
    void set_channel(vec2i p, int c, float t);

    T get_channel_raw(vec2i p, int c);
    float get_channel(vec2i p, int c);

    void set_pixel_raw(vec2i p, std::span<const T, channels> t);
    void set_pixel(vec2i p, std::span<const float, channels> t);

    std::array<T, channels> get_pixel_raw(vec2i p);
    std::array<float, channels> get_pixel(vec2i p);

    void write_png(const std::string& filename);

    int width() { return resolution.x; }
    int height() { return resolution.y; }

    vec2i dimensions() {
        return resolution;
    }

private:
    vec2i resolution;
    T* contents;
    std::unique_ptr<color_encoding> encoding;

    template <typename U, std::size_t... Is>
    static constexpr std::array<U, channels> fill_pixel(std::index_sequence<Is...>, const U& value) {
        return std::array<U, channels>({((void) Is, value)...});
    }
    template <typename U>
    static constexpr std::array<U, channels> fill_pixel(const U& value) {
        return fill_pixel(std::make_index_sequence<channels>(), value);
    }

    static constexpr T zero = encode_channel<T>(0.f);
    static constexpr std::array<T, channels> pixel_zero_internal = fill_pixel<T>(zero);
    static constexpr std::array<float, channels> pixel_zero = fill_pixel<float>(0.f);
};