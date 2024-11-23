#pragma once

#include <cmath>

inline float linear_to_srgb(float linear) {
    return std::pow(linear, 1.0f / 2.2f);
}

inline float srgb_to_linear(float srgb) {
    return std::pow(srgb, 2.2f);
}