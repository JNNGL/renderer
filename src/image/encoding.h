#pragma once

#include <math/util.h>
#include <color/color.h>

#include <span>
#include <cstdint>

class color_encoding {
public:
    virtual void to_linear(std::span<const uint8_t> in, std::span<float> out) const {
        for (int i = 0; i < in.size(); i++)
            out[i] = to_linear(float(in[i]) / 255.0f);
    }
    virtual void from_linear(std::span<const float> in, std::span<uint8_t> out) const {
        for (int i = 0; i < in.size(); i++)
            out[i] = (uint8_t) clamp(int(std::round(from_linear(in[i]) * 255.0f)), 0, 255);
    }

    virtual float to_linear(float f) const { return f; }
    virtual float from_linear(float f) const { return f; }
};

class srgb_color_encoding : public color_encoding {
public:
    float to_linear(float f) const override {
        return srgb_to_linear(f);
    }
    float from_linear(float f) const override {
        return linear_to_srgb(f);
    }
};