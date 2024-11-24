#include "transform.h"

transform translate(vec3f delta) {
    matrix<4> m(1, 0, 0, delta.x,
                0, 1, 0, delta.y,
                0, 0, 1, delta.z,
                0, 0, 0, 1);
    matrix<4> i(1, 0, 0, -delta.x,
                0, 1, 0, -delta.y,
                0, 0, 1, -delta.z,
                0, 0, 0, 1);
    return transform{m, i};
}

transform scale(vec3f scale) {
    matrix<4> m(scale.x, 0, 0, 0,
                0, scale.y, 0, 0,
                0, 0, scale.z, 0,
                0, 0, 0, 1);
    matrix<4> i(1 / scale.x, 0, 0, 0,
                0, 1 / scale.y, 0, 0,
                0, 0, 1 / scale.z, 0,
                0, 0, 0, 1);
    return transform{m, i};
}

transform rotate_x(float theta) {
    float rad = radians(theta);
    float sinTheta = std::sin(rad);
    float cosTheta = std::cos(rad);
    matrix<4> m(1, 0, 0, 0,
                0, cosTheta, -sinTheta, 0,
                0, sinTheta, cosTheta, 0,
                0, 0, 0, 1);
    return transform{m, transpose(m)};
}

transform rotate_y(float theta) {
    float rad = radians(theta);
    float sinTheta = std::sin(rad);
    float cosTheta = std::cos(rad);
    matrix<4> m(cosTheta, 0, sinTheta, 0,
                0, 1, 0, 0,
                -sinTheta, 0, cosTheta, 0,
                0, 0, 0, 1);
    return transform{m, transpose(m)};
}

transform rotate_z(float theta) {
    float rad = radians(theta);
    float sinTheta = std::sin(rad);
    float cosTheta = std::cos(rad);
    matrix<4> m(cosTheta, -sinTheta, 0, 0,
                sinTheta, cosTheta, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
    return transform{m, transpose(m)};
}

inline transform rotate(float sinTheta, float cosTheta, vec3f axis) {
    vec3f a = normalize(axis);
    matrix<4> m;
    m[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
    m[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
    m[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
    m[0][3] = 0;
    m[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
    m[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
    m[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
    m[1][3] = 0;
    m[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
    m[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
    m[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
    m[2][3] = 0;

    return transform{m, transpose(m)};
}

transform rotate(float theta, vec3f axis) {
    float rad = radians(theta);
    float sinTheta = std::sin(rad);
    float cosTheta = std::cos(rad);
    return rotate(sinTheta, cosTheta, axis);
}

transform rotate_from_to(vec3f from, vec3f to) {
    vec3f r;
    if (std::abs(from.x) < 0.72f && std::abs(to.x) < 0.72f) r = vec3f(1, 0, 0);
    else if (std::abs(from.y) < 0.72f && std::abs(to.y) < 0.72f) r = vec3f(0, 1, 0);
    else r = vec3f(0, 0, 1);

    vec3f u = r - from, v = r - to;
    matrix<4> res;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            res[i][j] = ((i == j) ? 1 : 0) - 2 / dot(u, u) * u[i] * u[j] -
                        2 / dot(v, v) * v[i] * v[j] +
                        4 * dot(u, v) / (dot(u, u) * dot(v, v)) * v[i] * u[j];

    return transform{res, transpose(res)};
}

transform look_at(vec3f pos, vec3f look, vec3f up) {
    up = normalize(up);

    matrix<4> m;
    m[0][3] = pos.x;
    m[1][3] = pos.y;
    m[2][3] = pos.z;
    m[3][3] = 1;

    vec3f dir = normalize(look - pos);
    vec3f cud = cross(up, dir);
    if (length(cud) == 0) {
        return {};
    }

    vec3f right = normalize(cud);
    up = cross(dir, right);
    m[0][0] = right.x;
    m[1][0] = right.y;
    m[2][0] = right.z;
    m[3][0] = 0;
    m[0][1] = up.x;
    m[1][1] = up.y;
    m[2][1] = up.z;
    m[3][1] = 0;
    m[0][2] = dir.x;
    m[1][2] = dir.y;
    m[2][2] = dir.z;
    m[3][2] = 0;

    return transform{m, *inverse(m)};
}

transform orthographic(float near, float far) {
    return scale(vec3f(1, 1, 1 / (far - near))) * translate(vec3f(0, 0, -near));
}

transform perspective(float fov, float near, float far) {
    matrix<4> p(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, far / (far - near), -far * near / (far - near),
                0, 0, 1, 0);
    float invTanAng = 1 / std::tan(radians(fov) / 2);
    return scale(vec3f(invTanAng, invTanAng, 1)) * transform(p);
}