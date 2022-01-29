#include "camera.h"
#include "hittable_list.h"
#include "material.h"
#include "math_utils.h"
#include "sphere.h"
#include "sys.h"
#include "utils.h"
#include "vec3.h"
#include <fstream>
#include <iostream>
#include <ostream>

color ray_color(const ray& r, const hittable& world, int depth) {
    if (depth < 0)
        return color(0, 0, 0);

    hit_record hr;
    if (world.hit(r, 0.001, infinity, hr)) {
        // naive
        // point3 target = hr.p + hr.normal + random_in_hemisphere(hr.normal);
        // cos^3(phi) distribution
        // point3 target = hr.p + hr.normal + random_in_unit_sphere();
        // cos(phi) distribution
        ray scattered;
        color attenuation;
        if (hr.mat_ptr->scatter(r, hr, attenuation, scattered)) {
            return attenuation * ray_color(scattered, world, depth - 1);
        }
        return color(0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    color c = lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 1.0), (unit_direction.y() + 1.0) * 0.5);
    return c;
}

void render() {
    auto R = cos(pi / 4);
    hittable_list world;

    auto material_left = std::make_shared<lambertian>(color(0, 0, 1));
    auto material_right = std::make_shared<lambertian>(color(1, 0, 0));

    world.add(std::make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(std::make_shared<sphere>(point3(R, 0, -1), R, material_right));

    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    camera cam(90.0, aspect_ratio);
    std::ofstream result_file_stream;
    result_file_stream.open("./image.ppm");
    std::ostream& res_stream = result_file_stream;

    res_stream << "P3\n" << image_width << " " << image_height << "\n256\n";
    for (int i = image_height - 1; i >= 0; i--) {
        std::cerr << "Remaining: " << i << std::endl;
        for (int j = 0; j < image_width; j++) {
            color pixel_color;
            for (int s = 0; s < samples_per_pixel; s++) {
                double u = static_cast<double>(j + random_double()) / (image_width - 1);
                double v = static_cast<double>(i + random_double()) / (image_height - 1);
                auto r = cam.cast_ray(u, v);

                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(res_stream, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "Done!" << std::endl;
    result_file_stream.close();
    open_preview("./image.ppm");
}

int main() { render(); }
