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
    color background;

    // Position and Orientation
    double vfov = 90;
    vec3 camera_center = point3(0, 0, 0); // also lookfrom
    point3 lookat = point3(0, 0, -1);
    vec3 vup = vec3(0, 1, 0); // relative up direction of camera

    double defocus_angle = 0; // Variation angle of rays through each pixel
    double focus_dist = 10;   // Distance from camera lookfrom point to plane of perfect focus

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
    vec3 first_pixel;
    vec3 delta_u, delta_v;
    vec3 u, v, w;
    vec3 defocus_disk_u; // Defocus disk horizontal radius
    vec3 defocus_disk_v; // Defocus disk vertical radius

    double pixel_samples_scale;

    void init()
    {
        image_height = static_cast<int>(image_width / aspect_ratio);
        if (image_height < 1)
            image_height = 1;

        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate u,v,w
        w = unit_vector(camera_center - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        vec3 viewport_u = viewport_width * u;   // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v; // Vector down viewport vertical edge

        delta_u = viewport_u / image_width;
        delta_v = viewport_v / image_height;

        first_pixel = camera_center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const
    {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        vec3 offset = sample_square();
        auto pixel_sample = first_pixel + ((i + offset.x) * delta_u) + ((j + offset.y) * delta_v);

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_square() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0, 0, 0);

        hit_record rec;

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

        return color_from_emission + color_from_scatter;
    }

    point3 defocus_disk_sample() const
    {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
    }
};

#endif
