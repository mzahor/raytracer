#include "camera.h"
#include "hittable_list.h"
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
        point3 target = hr.p + hr.normal + random_unit_vector();
        return 0.5 * ray_color(ray(hr.p, target - hr.p), world, depth - 1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    color c = lerp(color(1.0, 1.0, 1.0), color(0.5, 0.7, 1.0),
                   (unit_direction.y() + 1.0) * 0.5);
    return c;
}

void render() {
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    camera cam(aspect_ratio);
    std::ofstream result_file_stream;
    result_file_stream.open("./image.ppm");
    std::ostream& res_stream = result_file_stream;

    res_stream << "P3\n" << image_width << " " << image_height << "\n256\n";
    for (int i = image_height - 1; i >= 0; i--) {
        std::cerr << "Remaining: " << i << std::endl;
        for (int j = 0; j < image_width; j++) {
            color pixel_color;
            for (int s = 0; s < samples_per_pixel; s++) {
                double u = static_cast<double>(j + random_double()) /
                           (image_width - 1);
                double v = static_cast<double>(i + random_double()) /
                           (image_height - 1);
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
