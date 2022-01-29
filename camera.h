#ifndef CAMERA_H
#define CAMERA_H

#include "math_utils.h"
#include "ray.h"
#include "vec3.h"
#include <system_error>

class camera {
  public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double fov, double ratio, double aperture, double focus_dist)
        : aspect_ratio(ratio), fov(fov), aperture(aperture), focus_dist(focus_dist) {
        const double theta = deg_to_rad(fov);
        const double h = tan(theta / 2.0);
        const double viewport_height = h * 2.0;
        const double viewport_width = viewport_height * aspect_ratio;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        // auto v = unit_vector(cross(w, u));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = viewport_width * u * focus_dist;
        vertical = viewport_height * v * focus_dist;
        lower_left_corner = origin - focus_dist * w - horizontal / 2 - vertical / 2;

        lens_radius = aperture / 2;
    }

    ray cast_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

  private:
    double aspect_ratio;
    double fov;
    double aperture;
    double focus_dist;
    double lens_radius;
    point3 origin;
    vec3 u;
    vec3 v;
    vec3 w;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;
};

#endif
