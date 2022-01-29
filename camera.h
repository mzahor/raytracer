#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"
#include "ray.h"

class camera {
  public:
    camera(double fov, double ratio) : aspect_ratio(ratio), fov(fov) {
        const double theta = deg_to_rad(fov);
        const double h = tan(theta / 2.0);
        const double viewport_height = h * 2.0;
        const double viewport_width = viewport_height * aspect_ratio;

        const double focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0, 0);
        vertical = vec3(0, viewport_height, 0);
        lower_left_corner = origin - vec3(0, 0, focal_length) - horizontal / 2 - vertical / 2;
    }

    ray cast_ray(double u, double v) const {
        return ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
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
