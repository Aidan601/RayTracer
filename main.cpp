#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>
#include <cmath>

double hit_sphere(const point3 &center, double radius, const ray &r)
{
    double t;
    auto a = dot(r.direction, r.direction);
    auto h = dot(r.direction, center - r.origin);
    auto c = dot((center - r.origin), (center - r.origin)) - (radius * radius);
    double discriminant = (h * h) - (a * c);
    if (discriminant < 0) // 0 ROOTS
        return -1;
    else // 1 or 2 ROOTS (We only calculate the one the camera sees for now)
        t = (h - sqrt(discriminant)) / a;
    return t;
}

color ray_color(const ray &r)
{
    point3 center = point3(0, 0, -1);
    double t = hit_sphere(center, 0.5, r);
    if (t != -1)
    {
        point3 point = r.at(t);
        vec3 normal_vector = unit_vector(point - center);
        return 0.5 * (normal_vector + 1);
    }
    vec3 unit_direction = unit_vector(r.direction);
    auto a = 0.5 * (unit_direction.y + 1.0);
    auto value = (1 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1);
    return (value);
}

int main()
{
    bool output = true;
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;
    int image_height = image_width / aspect_ratio;
    if (image_height < 1)
        image_height = 1;

    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);

    vec3 camera_center = vec3(0, 0, 0);
    auto focal_length = 1.0;

    vec3 viewport_u = vec3(viewport_width, 0, 0);
    vec3 viewport_v = vec3(0, -viewport_height, 0);

    vec3 delta_u = viewport_u / image_width;
    vec3 delta_v = viewport_v / image_height;

    vec3 first_pixel = (camera_center - (viewport_u / 2) - (viewport_v / 2)) + (delta_u / 2) + (delta_v / 2) - vec3(0, 0, focal_length);
    // std::cout << first_pixel << std::endl;

    if (output)
    {
        // FILE OUTPUT
        std::cout << "P3" << std::endl;
        std::cout << image_width << " " << image_height << std::endl;
        std::cout << 255 << std::endl;
    }
    for (int i = 0; i < image_height; i++)
    {
        if (output)
            std::clog << "\rRows Remaining: " << image_height - i << std::flush;
        for (int j = 0; j < image_width; j++)
        {
            auto pixel_center = first_pixel + (j * delta_u) + (i * delta_v);
            ray cast = ray(camera_center, pixel_center - camera_center);
            color print_color = ray_color(cast);
            if (output)
                write_color(std::cout, print_color);
        }
    }
    std::clog << "\rDONE! \n";
}