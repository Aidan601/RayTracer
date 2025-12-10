#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer.h"
#include "hittable.h"
#include "material.h"

class camera
{
public:
    double aspect_ratio = 1.0; // Width over Height Ratio
    int image_width = 100;     // Pixel count of image width
    int samples_per_pixel = 10;
    int max_depth = 10;

    void render(const hittable &world)
    {
        init();

        // FILE OUTPUT
        std::cout << "P3" << std::endl;
        std::cout << image_width << " " << image_height << std::endl;
        std::cout << 255 << std::endl;
        for (int i = 0; i < image_height; i++)
        {
            std::clog << "\rRows Remaining: " << (image_height - i - 1) << "   " << std::flush;
            for (int j = 0; j < image_width; j++)
            {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    ray r = get_ray(j, i);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        std::clog << "\rDONE! \n";
    }

private:
    int image_height;
    double viewport_height = 2.0;
    vec3 camera_center = vec3(0, 0, 0);
    double focal_length = 1.0;

    vec3 first_pixel;
    vec3 delta_u;
    vec3 delta_v;

    double pixel_samples_scale;

    void init()
    {
        image_height = static_cast<int>(image_width / aspect_ratio);
        if (image_height < 1)
            image_height = 1;
        auto viewport_width = viewport_height * (double(image_width) / image_height);
        vec3 viewport_u = vec3(viewport_width, 0, 0);
        vec3 viewport_v = vec3(0, -viewport_height, 0);

        delta_u = viewport_u / image_width;
        delta_v = viewport_v / image_height;

        first_pixel = (camera_center - (viewport_u / 2) - (viewport_v / 2)) + (delta_u / 2) + (delta_v / 2) - vec3(0, 0, focal_length);

        pixel_samples_scale = 1.0 / samples_per_pixel;
    }

    ray get_ray(int i, int j) const
    {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        vec3 offset = sample_square();
        auto pixel_sample = first_pixel + ((i + offset.x) * delta_u) + ((j + offset.y) * delta_v);

        auto ray_origin = camera_center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray &r, int depth, const hittable &world)
    {
        if (depth <= 0) // If depth is below 0, no need to keep going
            return color(0, 0, 0);

        hit_record rec;
        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0);
        }
        vec3 unit_direction = unit_vector(r.direction);
        auto a = 0.5 * (unit_direction.y + 1.0);
        auto value = (1 - a) * color(1, 1, 1) + a * color(0.5, 0.7, 1);
        return value;
    }
};

#endif
