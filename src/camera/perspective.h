#pragma once

#include <camera/camera.h>
#include <math/transform.h>

class perspective_camera : public camera {
public:
    perspective_camera(vec2i resolution, float fov, transform camera_transform)
        : resolution(resolution), projection(perspective(fov, 0.05f, 1024.0f)),
          camera_transform(camera_transform) {}

    std::optional<ray> generate_ray(camera_sample_ctx ctx) const override;

private:
    vec2i resolution;
    transform projection;
    transform camera_transform;
};