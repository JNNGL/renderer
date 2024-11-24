#include "perspective.h"

std::optional<ray> perspective_camera::generate_ray(camera_sample_ctx ctx) const  {
    vec3f screenSpace = vec3f(float(ctx.pixel.x) / float(resolution.x), float(ctx.pixel.y) / float(resolution.y), 1.f);

    vec3f ndc = 2.f * screenSpace - vec3f(1.f);
    vec3f pointOnFarPlane = projection(ndc);

    auto origin = vec3f(0.f);
    vec3f direction = normalize(pointOnFarPlane - origin);

    return camera_transform(ray{origin, direction});
}