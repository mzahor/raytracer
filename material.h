#pragma once

#include "utils.h"

struct hit_record;

class material {
  public:
    virtual bool scatter(const ray& r_in, const hit_record& hr,
                         color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
  public:
    lambertian(const color& a) : albedo(a) {}
    bool scatter(const ray& r_in, const hit_record& hr, color& attenuation,
                 ray& scattered) const override {
        point3 scatter_direction = hr.normal + random_unit_vector();
        if (scatter_direction.near_zero())
            scatter_direction = hr.normal;
        scattered = ray(hr.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

  public:
    color albedo;
};

class metal : public material {
  public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}
    bool scatter(const ray& r_in, const hit_record& hr, color& attenuation,
                 ray& scattered) const override {
        auto reflected = reflect(r_in.direction(), hr.normal);
        scattered = ray(hr.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return dot(scattered.direction(), hr.normal) > 0;
    }

  public:
    color albedo;
    double fuzz;
};
