#include <iostream>
#include "vec3.hpp"
#include "ray.hpp"

void render_gradient();
void test();

int main()
{
    // render_gradient();
    test();
}

void test()
{
    ray r(point3(1, 2, 3), vec3(4, 5, 6));
    std::cout << r.at(2) << std::endl;
}

void render_gradient()
{
    const int image_width = 256;
    const int image_height = 256;
    std::cout << "P3\n"
              << image_height << " " << image_width << "\n256\n";
    for (int i = image_height; i >= 0; i--)
    {
        std::cerr << "Remaining: " << i << std::endl;
        for (int j = 0; j < image_width; j++)
        {
            auto col = color(
                static_cast<double>(j) / (image_width - 1),
                static_cast<double>(i) / (image_height - 1),
                0.25);
            write_color(std::cout, col);
        }
    }
    std::cerr << "Done!" << std::endl;
}
