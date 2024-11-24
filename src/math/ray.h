#pragma once

#include <math/vec.h>

class ray {
public:
    ray() = default;
    ray(vec3f p, vec3f d)
        : ro(p), rd(d) {}

    vec3f operator()(float t) const {
        return ro + t * rd;
    }

    vec3f origin() const { return ro; }
    vec3f direction() const { return rd; }

private:
    vec3f ro;
    vec3f rd;
};