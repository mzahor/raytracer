#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "ray.h"
#include "vec3.h"

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>().infinity();
const double pi = 3.1415926535897932385;

inline double deg_to_rad(double deg) { return deg * pi / 180.0; }

template <typename T> T lerp(T start, T end, double t) {
  return (1 - t) * start + t * end;
}

inline double sqr(double a) { return a * a; }

inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;
  return distribution(generator);
}

inline double clamp(double x, double min, double max) {
  if (x > max)
    return max;
  if (x < min)
    return min;
  return x;
}

inline int get_col(double val) {
  return static_cast<int>(clamp(val, 0, 0.999) * 256);
}

void write_color(std::ostream &out, const color &col,
                 double samples_per_pixel) {
  auto r = col.x();
  auto g = col.y();
  auto b = col.z();

  auto const scale = 1.0 / samples_per_pixel;

  r *= scale;
  g *= scale;
  b *= scale;

  out << get_col(r) << ' ' << get_col(g) << ' ' << get_col(b) << '\n';
}

#endif
