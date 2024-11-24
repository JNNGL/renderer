#pragma once

#include <math/vec.h>
#include <math/ray.h>

#include <optional>

struct camera_sample_ctx {
    vec2f pixel;
};

class camera {
public:
    virtual ~camera() = default;

    virtual std::optional<ray> generate_ray(camera_sample_ctx ctx) const = 0;
};