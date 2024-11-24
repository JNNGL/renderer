#pragma once

#include <math/mat.h>
#include <math/ray.h>

class transform {
public:
    transform() = default;
    transform(matrix<4> t, matrix<4> tInv)
        : m(t), mInv(tInv) {}
    transform(matrix<4> t)
        : m(t) {
        std::optional<matrix<4>> inv = inverse(t);
        if (inv) mInv = *inv;
        else {
            float NaN = std::numeric_limits<float>::signaling_NaN();
            mInv = {NaN, NaN, NaN, NaN,
                    NaN, NaN, NaN, NaN,
                    NaN, NaN, NaN, NaN,
                    NaN, NaN, NaN, NaN};
        }
    }

    bool operator==(const transform& t) const { return m == t.m; }
    bool operator!=(const transform& t) const { return m != t.m; }
    bool is_identity() const { return m.is_identity(); }

    matrix<4> get_matrix() const { return m; }
    matrix<4> get_inverse_matrix() const { return mInv; }

    template <typename T>
    auto apply(vec3<T> v, T w = 1) const -> vec3<decltype(T{} + float{})> {
        float xp = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
        float yp = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
        float zp = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
        if (w != 0) {
            xp += m[0][3] * w;
            yp += m[1][3] * w;
            zp += m[2][3] * w;
        }
        T wp = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * w;
        vec3<decltype(T{} + float{})> vp{xp, yp, zp};
        if (wp != 1) vp /= wp;
        return vp;
    }
    ray apply(const ray& r) const {
        return ray{apply(r.origin(), 1.f), apply(r.direction(), 0.f)};
    }

    template <typename T>
    vec3<T> apply_inverse(vec3<T> v, T w = 1) const {
        return transform{mInv, m}.apply<T>(v, w);
    }
    ray apply_inverse(const ray& r) const {
        return transform{mInv, m}.apply(r);
    }

    vec3f operator()(vec3f v) const {
        return apply(v);
    }
    ray operator()(const ray& r) const {
        return apply(r);
    }

    transform operator*(const transform& t) const {
        return {m * t.m, t.mInv * mInv};
    }

private:
    matrix<4> m, mInv;
};

inline transform inverse(const transform& t) {
    return transform{t.get_inverse_matrix(), t.get_matrix()};
}

transform translate(vec3f delta);
transform scale(vec3f scale);
transform rotate_x(float theta);
transform rotate_y(float theta);
transform rotate_z(float theta);
transform rotate(float theta, vec3f axis);
transform rotate_from_to(vec3f from, vec3f to);

transform look_at(vec3f pos, vec3f look, vec3f up);
transform orthographic(float near, float far);
transform perspective(float fov, float near, float far);