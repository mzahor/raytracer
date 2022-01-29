#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"
#include "ray.h"
#include "vec3.h"

class camera {
  public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double fov, double ratio) : aspect_ratio(ratio), fov(fov) {
        const double theta = deg_to_rad(fov);
        const double h = tan(theta / 2.0);
        const double viewport_height = h * 2.0;
        const double viewport_width = viewport_height * aspect_ratio;

        auto w = unit_vector(lookfrom - lookat);
        auto u = unit_vector(cross(vup, w));
        auto v = unit_vector(cross(w, u));

        origin = lookfrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - w - horizontal / 2 - vertical / 2;
    }

    ray cast_ray(double s, double t) const {
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

  private:
    double aspect_ratio;
    double fov;
    point3 origin;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;
};

#endif
