#pragma once

#include "utils.h"
#include "vec3.h"

struct hit_record;

class material {
  public:
    virtual bool scatter(const ray& r_in, const hit_record& hr, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
  public:
    lambertian(const color& a) : albedo(a) {}
    bool scatter(const ray& r_in, const hit_record& hr, color& attenuation, ray& scattered) const override {
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
    bool scatter(const ray& r_in, const hit_record& hr, color& attenuation, ray& scattered) const override {
        auto reflected = reflect(r_in.direction(), hr.normal);
        scattered = ray(hr.p, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return dot(scattered.direction(), hr.normal) > 0;
    }

  public:
    color albedo;
    double fuzz;
};

class dielectric : public material {
  public:
    dielectric(double refraction_index) : ri(refraction_index) {}

    virtual bool scatter(const ray& r_in, const hit_record& hr, color& attenuation, ray& scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = hr.front_face ? (1.0 / ri) : ri;
        vec3 unit_direction = unit_vector(r_in.direction());

        double cos_theta = fmin(dot(-unit_direction, hr.normal), 1.0);
        double sin_theta = sqrt(1 - sqr(cos_theta));
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        vec3 direction;

        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, hr.normal);
        } else {
            direction = refract(unit_direction, hr.normal, refraction_ratio);
        }
        scattered = ray(hr.p, direction);
        return true;
    }

  private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

  public:
    double ri;
};
