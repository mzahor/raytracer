#ifndef VEC3_H
#define VEC3_H

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

  public:
    double e[3];
};

using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& vec) {
    return out << vec.x() << ' ' << vec.y() << ' ' << vec.z();
}

inline vec3 operator+(const vec3& left, const vec3& right) {
    return vec3(left.x() + right.x(), left.y() + right.y(),
                left.z() + right.z());
}

inline vec3 operator-(const vec3& left, const vec3& right) {
    return vec3(left.x() - right.x(), left.y() - right.y(),
                left.z() - right.z());
}

inline vec3 operator*(const vec3& left, const vec3& right) {
    return vec3(left.x() * right.x(), left.y() * right.y(),
                left.z() * right.z());
}

inline vec3 operator*(double t, const vec3& vec) {
    return vec3(vec.x() * t, vec.y() * t, vec.z() * t);
}

inline vec3 operator*(const vec3& vec, double t) { return t * vec; }

inline vec3 operator/(const vec3& vec, double t) { return (1 / t) * vec; }

inline double dot(const vec3& left, const vec3& right) {
    return left.x() * right.x() + left.y() * right.y() + left.z() * right.z();
}

inline vec3 cross(const vec3& left, const vec3& right) {
    return vec3(left.e[1] * right.e[2] - left.e[2] * right.e[1],
                left.e[2] * right.e[0] - left.e[0] * right.e[2],
                left.e[0] * right.e[1] - left.e[1] * right.e[0]);
}

inline vec3 unit_vector(const vec3& vec) { return vec / vec.length(); }

#endif
