#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <memory>
#include <vector>

using std::shared_ptr;

class hittable_list : public hittable {
  private:
    std::vector<shared_ptr<hittable>> objects;

  public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> obj) { add(obj); }

    void add(shared_ptr<hittable> obj) { objects.push_back(obj); }

    void clear() { objects.clear(); }

    bool hit(const ray& r, double t_min, double t_max,
             hit_record& hr) const override;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max,
                        hit_record& hr) const {
    hit_record temp_hr;
    double closest_so_far = t_max;
    bool hit_any = false;

    for (const auto& obj : objects) {
        if (obj->hit(r, t_min, closest_so_far, temp_hr)) {
            hit_any = true;
            closest_so_far = temp_hr.t;
            hr = temp_hr;
        }
    }

    return hit_any;
}

#endif
