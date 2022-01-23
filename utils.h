#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>
#include <memory>

#include "ray.h"
#include "vec3.h"

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>().infinity();
const double pi = 3.1415926535897932385;

inline double deg_to_rad(double deg)
{
    return deg * pi / 180.0;
}

template <typename T>
T lerp(T start, T end, double t)
{
    return (1 - t) * start + t * end;
}

inline double sqr(double a)
{
    return a * a;
}

#endif
