#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>
#include <memory>

#include "math_utils.h"
#include "ray.h"
#include "vec3.h"

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

inline int get_col(double val) {
    return static_cast<int>(clamp(val, 0, 0.999) * 256);
}

void write_color(std::ostream& out, const color& col,
                 double samples_per_pixel) {
    auto r = col.x();
    auto g = col.y();
    auto b = col.z();

    auto const scale = 1.0 / samples_per_pixel;

    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    out << get_col(r) << ' ' << get_col(g) << ' ' << get_col(b) << '\n';
}

#endif
