#pragma once

#include <cmath>

template <typename T>
inline T clamp(T t, T a, T b) {
    return std::min(std::max(t, a), b);
}