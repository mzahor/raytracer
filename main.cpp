#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"

void render_gradient();
void test();

template <typename T>
T lerp(T start, T end, double t)
{
    return (1 - t) * start + t * end;
}

inline double sqr(double a)
{
    return a * a;
}

bool hits_sphere(const ray &r, point3 center, double radius)
{
    auto b = r.direction();
    auto a = r.origin();
    auto oc = r.origin() - center;
    double d = sqr(dot(2 * b, oc)) - 4 * (dot(b, b) * dot(oc, oc) - sqr(radius));
    return d >= 0;
}

color ray_color(const ray &r)
{
    vec3 unit_direction = unit_vector(r.direction());
    if (hits_sphere(r, point3(0, 0, 1), 0.5))
        return color(1, 0, 0);
    // vec3 unit_direction = r.direction() / 2.0;
    color c = lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 1.0), (unit_direction.y() + 1.0) * 0.5);
    // std::cerr << unit_direction << ' ' <<  (unit_direction.y() + 1.0) * 0.5<< '\n';
    // std::cin.get();
    return c;
}

int main()
{
    render_gradient();
    // test();
}

void test()
{
    ray r(point3(1, 2, 3), vec3(4, 5, 6));
    std::cout << r.at(2) << std::endl;
}

void render_gradient()
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
