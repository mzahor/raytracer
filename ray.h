#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
public:
  ray(const point3 &origin, const vec3 &direction)
      : orig(origin), dir(direction) {}

  point3 at(double t) const { return orig + dir * t; }

  vec3 origin() const { return orig; }
  vec3 direction() const { return dir; }

public:
  point3 orig;
  vec3 dir;
};

#endif
