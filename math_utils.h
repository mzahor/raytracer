#pragma once

#include <random>

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

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x > max)
        return max;
    if (x < min)
        return min;
    return x;
}
