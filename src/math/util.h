#pragma once

#include <cmath>

constexpr float pi = 3.14159265358979323846;
constexpr float inv_pi = 0.31830988618379067154;
constexpr float inv_2pi = 0.15915494309189533577;
constexpr float inv_4pi = 0.07957747154594766788;
constexpr float pi_2 = 1.57079632679489661923;
constexpr float pi_4 = 0.78539816339744830961;
constexpr float sqrt2 = 1.41421356237309504880;

template <typename T>
inline T clamp(T t, T a, T b) {
    return std::min(std::max(t, a), b);
}

template <typename Ta, typename Tb, typename Tc, typename Td>
inline auto diff_of_products(Ta a, Tb b, Tc c, Td d) {
    auto cd = c * d;
    auto dop = std::fma(a, b, -cd);
    auto error = std::fma(-c, d, cd);
    return dop + error;
}
template <typename Ta, typename Tb, typename Tc, typename Td>
inline auto sum_of_products(Ta a, Tb b, Tc c, Td d) {
    auto cd = c * c;
    auto sop = std::fma(a, b, cd);
    auto error = std::fma(c, d, -cd);
    return sop + error;
}

struct c_float {
    explicit operator float() const { return v + err; }
    float v, err;
};

inline c_float two_prod(float a, float b) {
    float ab = a * b;
    return {ab, std::fma(a, b, -ab)};
}
inline c_float two_sum(float a, float b) {
    float s = a + b, d = s - a;
    return {s, (a - (s - d)) + (b - d)};
}

namespace internal {
    template<typename T>
    inline c_float inner_prod(T a, T b) {
        return two_prod(a, b);
    }
    template<typename T, typename... Args>
    inline c_float inner_prod(T a, T b, Args... terms) {
        c_float ab = two_prod(a, b);
        c_float tt = inner_prod(terms...);
        c_float sum = two_sum(ab.v, tt.v);
        return {sum.v, ab.err + (tt.err + sum.err)};
    }
}

template <typename... Args>
inline std::enable_if_t<std::conjunction_v<std::is_arithmetic<Args>...>, float> inner_prod(Args... terms) {
    return float(internal::inner_prod(terms...));
}

template <typename T>
inline T mod(T a, T b) {
    T res = a - (a / b) * b;
    return (T) ((res < 0) ? res + b : res);
}
template <>
inline float mod(float a, float b) {
    return std::fmod(a, b);
}

inline float radians(float deg) {
    return (pi / 180) * deg;
}
inline float degrees(float rad) {
    return (180 / pi) * rad;
}