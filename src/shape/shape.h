#pragma once

#include <math/ray.h>
#include <common.h>

#include <optional>

struct shape_isect {
    vec3f p;
    vec3f n;
    float t;
};

class shape {
public:
    virtual ~shape() = default;

    virtual std::optional<shape_isect> intersect(const ray& ray, float tMax = infinity) const = 0;
    virtual bool intersects(const ray& ray, float tMax = infinity) {
        return intersect(ray, tMax).has_value();
    }
};