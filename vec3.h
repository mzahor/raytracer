#ifndef VEC3_H
#define VEC3_H

#include "math_utils.h"
#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
  public:
    vec3() : e{0, 0, 0} {}
    vec3(double x, double y, double z) : e{x, y, z} {}

    inline double x() const { return e[0]; }
    inline double y() const { return e[1]; }
    inline double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

    vec3& operator+=(const vec3& other) {
        e[0] += other.x();
        e[1] += other.y();
        e[2] += other.z();
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        *this *= 1 / t;
        return *this;
    }

    double length() const { return sqrt(length_squared()); }

    double length_squared() const { return x() * x() + y() * y() + z() * z(); }

    inline static vec3 random() { return vec3(random_double(), random_double(), random_double()); }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool near_zero() const {
        const double s = 1e-8;
        return fabs(x()) < s && fabs(y()) < s && fabs(z()) < s;
    }

  public:
    double e[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& vec) {
    return out << vec.x() << ' ' << vec.y() << ' ' << vec.z();
}

inline vec3 operator+(const vec3& left, const vec3& right) {
    return vec3(left.x() + right.x(), left.y() + right.y(), left.z() + right.z());
}

inline vec3 operator-(const vec3& left, const vec3& right) {
    return vec3(left.x() - right.x(), left.y() - right.y(), left.z() - right.z());
}

inline vec3 operator*(const vec3& left, const vec3& right) {
    return vec3(left.x() * right.x(), left.y() * right.y(), left.z() * right.z());
}

inline vec3 operator*(double t, const vec3& vec) { return vec3(vec.x() * t, vec.y() * t, vec.z() * t); }

inline vec3 operator*(const vec3& vec, double t) { return t * vec; }

inline vec3 operator/(const vec3& vec, double t) { return (1 / t) * vec; }

inline double dot(const vec3& left, const vec3& right) {
    return left.x() * right.x() + left.y() * right.y() + left.z() * right.z();
}

inline vec3 cross(const vec3& left, const vec3& right) {
    return vec3(left.e[1] * right.e[2] - left.e[2] * right.e[1], left.e[2] * right.e[0] - left.e[0] * right.e[2],
                left.e[0] * right.e[1] - left.e[1] * right.e[0]);
}

inline vec3 unit_vector(const vec3& vec) { return vec / vec.length(); }

vec3 random_in_unit_sphere() {
    while (true) {
        auto v = vec3::random(-1, 1);
        if (v.length_squared() >= 1)
            continue;
        return v;
    }
}

vec3 random_in_unit_disk() {
    while (true) {
        auto v = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (v.length_squared() >= 1)
            continue;
        return v;
    }
}

vec3 reflect(const vec3& v, const vec3 n) {
    // return v - 2 * dot(v, n) / dot(n, n) * n;
    // since n is a unit vector, dot(n, n) == 1
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& v, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-v, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (v + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

inline vec3 random_unit_vector() { return unit_vector(random_in_unit_sphere()); }

inline vec3 random_in_hemisphere(const vec3& normal) {
    auto r = random_in_unit_sphere();
    return dot(r, normal) > 0 ? r : -r;
}

#endif
