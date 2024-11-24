#pragma once

#include <cmath>

template <typename T>
class vec2 {
public:
    vec2() = default;
    explicit vec2(T t) : vec2(t, t) {}
    vec2(T x, T y) : x(x), y(y) {}

    T operator[](int i) const {
        return i == 0 ? x : y;
    }
    T& operator[](int i) {
        return i == 0 ? x : y;
    }

    vec2<T> operator-() const {
        return {-x, -y};
    }

    bool operator==(vec2<T> t) const {
        return x == t.x && y == t.y;
    }
    bool operator!=(vec2<T> t) const {
        return x != t.x || y != t.y;
    }

    template <typename I>
    auto operator+(vec2<I> t) const -> vec2<decltype(T{} + I{})> {
        return {x + t.x, y + t.x};
    }
    template <typename I>
    vec2<T>& operator+=(vec2<I> t) {
        x += t.x;
        y += t.y;
        return *this;
    }

    template <typename I>
    auto operator-(vec2<I> t) const -> vec2<decltype(T{} - I{})> {
        return {x - t.x, y - t.y};
    }
    template <typename I>
    vec2<T>& operator-=(vec2<I> t) {
        x -= t.x;
        y -= t.y;
        return *this;
    }

    template <typename I>
    auto operator*(I t) const -> vec2<decltype(T{} * I{})> {
        return {x * t, y * t};
    }
    template <typename I>
    vec2<T>& operator*=(I t) {
        x *= t;
        y *= t;
        return *this;
    }

    template <typename I>
    auto operator/(I t) const -> vec2<decltype(T{} / I{})> {
        I invT = 1 / t;
        return {x * invT, y * invT};
    }
    template <typename I>
    vec2<T>& operator/=(I t) {
        I invT = 1 / t;
        return (*this) *= invT;
    }

public:
    T x, y;
};

template <typename T, typename I>
inline auto operator*(I i, vec2<T> t) -> vec2<decltype(I{} * T{})> {
    return t * i;
}

template <typename T>
inline vec2<T> abs(vec2<T> t) {
    return {std::abs(t.x), std::abs(t.y)};
}
template <typename T>
inline vec2<T> floor(vec2<T> t) {
    return {std::floor(t.x), std::floor(t.y)};
}
template <typename T>
inline vec2<T> ceil(vec2<T> t) {
    return {std::ceil(t.x), std::ceil(t.y)};
}
template <typename T>
inline vec2<T> min(vec2<T> t0, vec2<T> t1) {
    return {std::min(t0.x, t1.x), std::min(t0.y, t1.y)};
}
template <typename T>
inline vec2<T> max(vec2<T> t0, vec2<T> t1) {
    return {std::max(t0.x, t1.x), std::max(t0.y, t1.y)};
}
template <typename T>
inline T min_of(vec2<T> t0) {
    return std::min(t0.x, t0.y);
}
template <typename T>
inline T max_of(vec2<T> t0) {
    return std::max(t0.x, t0.y);
}
template <typename T>
inline auto mix(vec2<T> t0, vec2<T> t1, float a) -> vec2<decltype(T{} * a)> {
    return (1 - a) * t0 + a * t1;
}

using vec2f = vec2<float>;
using vec2i = vec2<int>;

template <typename T>
class vec3 {
public:
    vec3() = default;
    vec3(T t) : vec3(t, t, t) {}
    vec3(T x, T y, T z)
        : x(x), y(y), z(z) {}

    T operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }
    T& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    bool operator==(vec3<T> t) const {
        return x == t.x && y == t.y && z == t.z;
    }
    bool operator!=(vec3<T> t) const {
        return x != t.x || y != t.y || z != t.z;
    }

    vec3<T> operator-() const {
        return {-x, -y, -z};
    }

    template <typename I>
    auto operator+(vec3<I> t) const -> vec3<decltype(T{} + I{})> {
        return {x + t.x, y + t.x, z + t.z};
    }
    template <typename I>
    vec3<T>& operator+=(vec3<I> t) {
        x += t.x;
        y += t.y;
        z += t.z;
        return *this;
    }

    template <typename I>
    auto operator-(vec3<I> t) const -> vec3<decltype(T{} - I{})> {
        return {x - t.x, y - t.y, z - t.z};
    }
    template <typename I>
    vec3<T>& operator-=(vec3<I> t) {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }

    template <typename I>
    auto operator*(I t) const -> vec3<decltype(T{} * I{})> {
        return {x * t, y * t, z * t};
    }
    template <typename I>
    vec3<T>& operator*=(I t) {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }

    template <typename I>
    auto operator/(I t) const -> vec3<decltype(T{} / I{})> {
        float invT = 1 / t;
        return {x * invT, y * invT, z * invT};
    }
    template <typename I>
    vec3<T>& operator/=(I t) {
        I invT = 1 / t;
        return (*this) *= invT;
    }

public:
    T x, y, z;
};

template <typename T, typename I>
inline auto operator*(I i, vec3<T> t) -> vec3<decltype(I{} * T{})> {
    return t * i;
}

template <typename T>
inline vec3<T> abs(vec3<T> t) {
    return {std::abs(t.x), std::abs(t.y), std::abs(t.z)};
}
template <typename T>
inline vec3<T> floor(vec3<T> t) {
    return {std::floor(t.x), std::floor(t.y), std::floor(t.z)};
}
template <typename T>
inline vec3<T> ceil(vec3<T> t) {
    return {std::ceil(t.x), std::ceil(t.y), std::ceil(t.z)};
}
template <typename T>
inline vec3<T> min(vec3<T> t0, vec3<T> t1) {
    return {std::min(t0.x, t1.x), std::min(t0.y, t1.y), std::min(t0.z, t1.z)};
}
template <typename T>
inline vec3<T> max(vec3<T> t0, vec3<T> t1) {
    return {std::max(t0.x, t1.x), std::max(t0.y, t1.y), std::max(t0.z, t1.z)};
}
template <typename T>
inline T min_of(vec3<T> t0) {
    return std::min({t0.x, t0.y, t0.z});
}
template <typename T>
inline T max_of(vec3<T> t0) {
    return std::max({t0.x, t0.y, t0.z});
}
template <typename T>
inline auto mix(vec3<T> t0, vec3<T> t1, float a) -> vec3<decltype(T{} * a)> {
    return (1 - a) * t0 + a * t1;
}
template <typename T>
inline vec3<T> cross(vec3<T> u, vec3<T> v) {
    return {u[1] * v[2] - u[2] * v[1],
            u[2] * v[0] - u[0] * v[2],
            u[0] * v[1] - u[1] * v[0]};
}

using vec3f = vec3<float>;
using vec3i = vec3<int>;

template <typename T, typename V>
T dot(V v0, V v1);
template <typename T>
inline T dot(vec2<T> v0, vec2<T> v1) {
    return v0.x * v1.x + v0.y * v1.y;
}
template <typename T>
inline T dot(vec3<T> v0, vec3<T> v1) {
    return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}

template <typename T, typename V>
inline T abs_dot(V v0, V v1) {
    return std::abs(dot(v0, v1));
}
template <typename V>
inline auto length_sqr(V v) {
    return dot(v, v);
}
template <typename V>
inline auto length(V v) {
    return std::sqrt(length_sqr(v));
}
template <typename T, typename V>
inline T distance_sqr(V v0, V v1) {
    return length_sqr(v0 - v1);
}
template <typename T, typename V>
inline T distance(V v0, V v1) {
    return length(v0 - v1);
}
template <typename V>
inline V normalize(V v) {
    return v / length(v);
}