#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"
#include "utils.h"
#include <cstdio>
#include <memory>

class sphere : public hittable {
  private:
    point3 center;
    double radius;
    std::shared_ptr<material> mat_ptr;

  public:
    sphere() {}
    sphere(point3 c, double r, std::shared_ptr<material> m) : center(c), radius(r), mat_ptr(m) {}

    bool hit(const ray& r, double t_min, double t_max, hit_record& hr) const override;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& hr) const {
    auto oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - sqr(radius);
    double d = sqr(half_b) - a * c;

    if (d < 0.0)
        return false;

    auto t = (-half_b - sqrt(d)) / a;
    if (t > t_max || t < t_min) {
        t = (-half_b + sqrt(d)) / a;
        if (t > t_max || t < t_min)
            return false;
    }

    hr.t = t;
    hr.p = r.at(t);
    vec3 out_normal = (hr.p - center) / radius;
    hr.set_normal(r, out_normal);
    hr.mat_ptr = mat_ptr;

    return true;
}

#endif
