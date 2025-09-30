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
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Viewport widths less than one are ok since they are real valued.
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
}