#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

color ray_color(const ray &r)
{
    return color(0, 0, 0);
}

int main()
{
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = image_width / aspect_ratio;
    if (image_height < 1)
        image_height = 1;

    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (image_width / image_height);

    vec3 camera_center = vec3(0, 0, 0);
    auto focal_length = 1.0;

    // FILE OUTPUT
    std::cout << "P3" << std::endl;
    std::cout << image_width << " " << image_height << std::endl;
    std::cout << 255 << std::endl;
    for (int i = 0; i < image_height; i++)
    {
        std::clog << "\rRows Remaining: " << image_height - i << std::flush;
        for (int j = 0; j < image_width; j++)
        {
            std::cout << color(255, 255, 255) << std::endl;
        }
    }
    std::clog << "\rDONE! \n";
}