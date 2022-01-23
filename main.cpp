#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"

void render();

template <typename T>
T lerp(T start, T end, double t)
{
    return (1 - t) * start + t * end;
}

inline double sqr(double a)
{
    return a * a;
}

double hit_sphere(const ray &r, point3 center, double radius)
{
    auto oc = r.origin() - center;
    auto a = r.direction().length_squared();
    auto half_b = dot(r.direction(), oc);
    auto c = oc.length_squared() - sqr(radius);
    double d = sqr(half_b) - a * c;
    if (d < 0.0)
        return -1.0;
    auto t = (-half_b - sqrt(d)) / a;
    return t;
}

color ray_color(const ray &r)
{
    vec3 unit_direction = unit_vector(r.direction());
    point3 center(0, 0, -1);
    auto t = hit_sphere(r, center, 0.5);

    if (t > 0.0)
    {
        auto n = unit_vector(r.at(t) - center);
        return color(n.x() + 1, n.y() + 1, n.z() + 1) * 0.5;
    }
    color c = lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 1.0), (unit_direction.y() + 1.0) * 0.5);
    return c;
}

int main()
{
    render();
}

void render()
{
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    const double viewport_height = 2.0;
    const double viewport_width = viewport_height * aspect_ratio;
    const double focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - vec3(0, 0, focal_length) - horizontal / 2 - vertical / 2;
    std::cerr << "Lower left: " << lower_left_corner << '\n';

    std::cout << "P3\n"
              << image_width << " " << image_height << "\n256\n";
    for (int i = image_height - 1; i >= 0; i--)
    {
        std::cerr << "Remaining: " << i << std::endl;
        for (int j = 0; j < image_width; j++)
        {
            double u = static_cast<double>(j) / (image_width - 1);
            double v = static_cast<double>(i) / (image_height - 1);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "Done!" << std::endl;
}
