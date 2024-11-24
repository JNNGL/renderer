#pragma once

#include <math/vec.h>
#include <math/util.h>

#include <optional>
#include <span>

namespace internal {
    template <int N>
    inline void fill_matrix(float m[N][N], int i, int j) {}
    template <int N, typename... Args>
    inline void fill_matrix(float m[N][N], int i, int j, float v, Args... args) {
        m[i][j] = v;
        if (++j == N) {
            ++i;
            j = 0;
        }
        fill_matrix<N>(m, i, j, args...);
    }
}

template <int N>
class matrix {
public:
    static constexpr matrix zero() {
        matrix m;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                m.m[i][j] = 0;
        return m;
    }

    matrix() {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                m[i][j] = (i == j) ? 1 : 0;
    }
    matrix(const float mat[N][N]) {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                m[i][j] = mat[i][j];
    }
    matrix(std::span<const float> t) {
        for (int i = 0; i < N * N; ++i)
            m[i / N][i % N] = t[i];
    }
    template <typename... Args>
    matrix(float v, Args... args) {
        internal::fill_matrix(m, 0, 0, v, args...);
    }

    matrix<N> operator+(const matrix<N>& t) const {
        matrix<N> r;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                r.m[i][j] = m[i][j] + t.m[i][j];
        return r;
    }
    matrix<N> operator*(float t) const {
        matrix<N> r;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                r.m[i][j] = m[i][j] * t;
        return r;
    }
    matrix<N> operator/(float t) const {
        matrix<N> r;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                r.m[i][j] = m[i][j] / t;
        return r;
    }

    bool operator==(const matrix<N>& t) const {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (m[i][j] != t.m[i][j])
                    return false;
        return true;
    }
    bool operator!=(const matrix<N>& t) const {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (m[i][j] != t.m[i][j])
                    return true;
        return false;
    }
    bool operator<(const matrix<N>& t) const {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                if (m[i][j] < t.m[i][j])
                    return true;
                if (m[i][j] > t.m[i][j])
                    return false;
            }
        return false;
    }

    std::span<const float> operator[](int i) const { return m[i]; }
    std::span<float> operator[](int i) { return std::span<float>(m[i]); }

    inline bool is_identity() const {
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                if (m[i][j] != ((i == j) ? 1 : 0))
                    return false;
            }
        return true;
    }

private:
    float m[N][N];
};

template <int N>
inline matrix<N> operator*(float t, const matrix<N>& m) {
    return m * t;
}

template <typename Tr, int N, typename T>
Tr operator*(const matrix<N>& m, const T& v) {
    Tr res;
    for (int i = 0; i < N; ++i) {
        res[i] = 0;
        for (int j = 0; j < N; ++j)
            res[i] += m[i][j] * v[j];
    }
    return res;
}

template <int N>
float determinant(const matrix<N>& m);
template <>
inline float determinant(const matrix<1>& m) {
    return m[0][0];
}
template <>
inline float determinant(const matrix<2>& m) {
    return diff_of_products(m[0][0], m[1][1], m[0][1], m[1][0]);
}
template <>
inline float determinant(const matrix<3>& m) {
    float minor12 = diff_of_products(m[1][1], m[2][2], m[1][2], m[2][1]);
    float minor02 = diff_of_products(m[1][0], m[2][2], m[1][2], m[2][0]);
    float minor01 = diff_of_products(m[1][0], m[2][1], m[1][1], m[2][0]);
    return std::fma(m[0][2], minor01, diff_of_products(m[0][0], minor12, m[0][1], minor02));
}
template <>
inline float determinant(const matrix<4>& m) {
    float s0 = diff_of_products(m[0][0], m[1][1], m[1][0], m[0][1]);
    float s1 = diff_of_products(m[0][0], m[1][2], m[1][0], m[0][2]);
    float s2 = diff_of_products(m[0][0], m[1][3], m[1][0], m[0][3]);
    float s3 = diff_of_products(m[0][1], m[1][2], m[1][1], m[0][2]);
    float s4 = diff_of_products(m[0][1], m[1][3], m[1][1], m[0][3]);
    float s5 = diff_of_products(m[0][2], m[1][3], m[1][2], m[0][3]);
    float c0 = diff_of_products(m[2][0], m[3][1], m[3][0], m[2][1]);
    float c1 = diff_of_products(m[2][0], m[3][2], m[3][0], m[2][2]);
    float c2 = diff_of_products(m[2][0], m[3][3], m[3][0], m[2][3]);
    float c3 = diff_of_products(m[2][1], m[3][2], m[3][1], m[2][2]);
    float c4 = diff_of_products(m[2][1], m[3][3], m[3][1], m[2][3]);
    float c5 = diff_of_products(m[2][2], m[3][3], m[3][2], m[2][3]);
    return (diff_of_products(s0, c5, s1, c4) +
            diff_of_products(s2, c3, -s3, c2) +
            diff_of_products(s5, c0, s4, c1));
}
template <int N>
inline float determinant(const matrix<N>& m) {
    matrix<N - 1> sub;
    float det = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N - 1; ++j) {
            for (int k = 0; k < N - 1; ++k)
                sub[j][k] = m[j + 1][k < i ? k : k + 1];
        }
        float sign = (i & 1) ? -1 : 1;
        det += sign * m[0][i] * determinant(sub);
    }
    return det;
}

template <int N>
inline matrix<N> transpose(const matrix<N>& m) {
    matrix<N> r;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            r[i][j] = m[j][i];
    return r;
}

template <int N>
std::optional<matrix<N>> inverse(const matrix<N>&);
template <>
inline std::optional<matrix<3>> inverse(const matrix<3>& m) {
    float det = determinant(m);
    if (det == 0) return {};
    float invDet = 1 / det;

    matrix<3> r;
    r[0][0] = invDet * diff_of_products(m[1][1], m[2][2], m[1][2], m[2][1]);
    r[1][0] = invDet * diff_of_products(m[1][2], m[2][0], m[1][0], m[2][2]);
    r[2][0] = invDet * diff_of_products(m[1][0], m[2][1], m[1][1], m[2][0]);
    r[0][1] = invDet * diff_of_products(m[0][2], m[2][1], m[0][1], m[2][2]);
    r[1][1] = invDet * diff_of_products(m[0][0], m[2][2], m[0][2], m[2][0]);
    r[2][1] = invDet * diff_of_products(m[0][1], m[2][0], m[0][0], m[2][1]);
    r[0][2] = invDet * diff_of_products(m[0][1], m[1][2], m[0][2], m[1][1]);
    r[1][2] = invDet * diff_of_products(m[0][2], m[1][0], m[0][0], m[1][2]);
    r[2][2] = invDet * diff_of_products(m[0][0], m[1][1], m[0][1], m[1][0]);
    return r;
}
template <>
inline std::optional<matrix<4>> inverse(const matrix<4>& m) {
    // https://github.com/google/ion/blob/master/ion/math/matrixutils.cc,
    // (c) Google, Apache license.
    float s0 = diff_of_products(m[0][0], m[1][1], m[1][0], m[0][1]);
    float s1 = diff_of_products(m[0][0], m[1][2], m[1][0], m[0][2]);
    float s2 = diff_of_products(m[0][0], m[1][3], m[1][0], m[0][3]);
    float s3 = diff_of_products(m[0][1], m[1][2], m[1][1], m[0][2]);
    float s4 = diff_of_products(m[0][1], m[1][3], m[1][1], m[0][3]);
    float s5 = diff_of_products(m[0][2], m[1][3], m[1][2], m[0][3]);
    float c0 = diff_of_products(m[2][0], m[3][1], m[3][0], m[2][1]);
    float c1 = diff_of_products(m[2][0], m[3][2], m[3][0], m[2][2]);
    float c2 = diff_of_products(m[2][0], m[3][3], m[3][0], m[2][3]);
    float c3 = diff_of_products(m[2][1], m[3][2], m[3][1], m[2][2]);
    float c4 = diff_of_products(m[2][1], m[3][3], m[3][1], m[2][3]);
    float c5 = diff_of_products(m[2][2], m[3][3], m[3][2], m[2][3]);

    float determinant = inner_prod(s0, c5, -s1, c4, s2, c3, s3, c2, s5, c0, -s4, c1);
    if (determinant == 0) return {};
    float s = 1 / determinant;

    float inv[4][4] = {{s * inner_prod(m[1][1], c5, m[1][3], c3, -m[1][2], c4),
                        s * inner_prod(-m[0][1], c5, m[0][2], c4, -m[0][3], c3),
                        s * inner_prod(m[3][1], s5, m[3][3], s3, -m[3][2], s4),
                        s * inner_prod(-m[2][1], s5, m[2][2], s4, -m[2][3], s3)},

                       {s * inner_prod(-m[1][0], c5, m[1][2], c2, -m[1][3], c1),
                        s * inner_prod(m[0][0], c5, m[0][3], c1, -m[0][2], c2),
                        s * inner_prod(-m[3][0], s5, m[3][2], s2, -m[3][3], s1),
                        s * inner_prod(m[2][0], s5, m[2][3], s1, -m[2][2], s2)},

                       {s * inner_prod(m[1][0], c4, m[1][3], c0, -m[1][1], c2),
                        s * inner_prod(-m[0][0], c4, m[0][1], c2, -m[0][3], c0),
                        s * inner_prod(m[3][0], s4, m[3][3], s0, -m[3][1], s2),
                        s * inner_prod(-m[2][0], s4, m[2][1], s2, -m[2][3], s0)},

                       {s * inner_prod(-m[1][0], c3, m[1][1], c1, -m[1][2], c0),
                        s * inner_prod(m[0][0], c3, m[0][2], c0, -m[0][1], c1),
                        s * inner_prod(-m[3][0], s3, m[3][1], s1, -m[3][2], s0),
                        s * inner_prod(m[2][0], s3, m[2][2], s0, -m[2][1], s1)}};

    return matrix<4>(inv);
}

template <>
inline matrix<3> operator*(const matrix<3>& m1, const matrix<3>& m2) {
    matrix<3> r;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            r[i][j] = inner_prod(m1[i][0], m2[0][j], m1[i][1], m2[1][j], m1[i][2], m2[2][j]);
    return r;
}
template <>
inline matrix<4> operator*(const matrix<4>& m1, const matrix<4>& m2) {
    matrix<4> r;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            r[i][j] = inner_prod(m1[i][0], m2[0][j], m1[i][1], m2[1][j], m1[i][2],
                                 m2[2][j], m1[i][3], m2[3][j]);
    return r;
}
template <int N>
inline matrix<N> operator*(const matrix<N>& m1, const matrix<N>& m2) {
    matrix<N> r;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            r[i][j] = 0;
            for (int k = 0; k < N; ++k)
                r[i][j] = std::fma(m1[i][k], m2[k][j], r[i][j]);
        }
    return r;
}

extern template class matrix<2>;
extern template class matrix<3>;
extern template class matrix<4>;