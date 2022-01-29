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

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

void render() {
    // Image

    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // World

    auto world = random_scene();

    // Camera

    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

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
