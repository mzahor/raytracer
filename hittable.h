#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record
{
    point3 p;
    vec3 normal;
    double t;
    bool front_face;

    inline void set_normal(const ray &r, const vec3 &out_normal)
    {
        front_face = dot(r.direction(), out_normal) < 0;
        normal = front_face ? out_normal : -out_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &hr) const = 0;
};

#endif
