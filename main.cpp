#include <iostream>
#include "utils.h"
#include "hittable_list.h"
#include "sphere.h"

color ray_color(const ray &r, const hittable &world)
{
    hit_record hr;
    if (world.hit(r, 0, infinity, hr))
    {
        return 0.5 * (hr.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    color c = lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 1.0), (unit_direction.y() + 1.0) * 0.5);
    return c;
}

void render()
{
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 680;
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

            color pixel_color = ray_color(r, world);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "Done!" << std::endl;
}

int main()
{
    render();
}
