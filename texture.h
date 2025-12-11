#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "raytracer.h"
#include "rtw_stb_image.h"

class texture
{
public:
    virtual ~texture() = default;

    virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture
{
public:
    solid_color(const color &albedo) : albedo(albedo) {}

    solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

    color value(double u, double v, const point3 &p) const override
    {
        return albedo;
    }

private:
    color albedo;
};

class checker_texture : public texture
{
public:
    checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
        : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_texture(double scale, const color &c1, const color &c2)
        : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

    color value(double u, double v, const point3 &p) const override
    {
        auto xInteger = int(std::floor(inv_scale * p.x));
        auto yInteger = int(std::floor(inv_scale * p.y));
        auto zInteger = int(std::floor(inv_scale * p.z));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class image_texture : public texture
{
public:
    image_texture()
        : image(), u_offset(0.0), v_offset(0.0)
    {
    }

    image_texture(const char *filename)
        : image(filename), u_offset(0.0), v_offset(0.0)
    {
    }

    image_texture(const char *filename, double u_off, double v_off)
        : image(filename), u_offset(u_off), v_offset(v_off)
    {
    }

    color value(double u, double v, const point3 &p) const override
    {
        if (image.height() <= 0)
            return color(0, 1, 1);

        // Apply offsets BEFORE wrapping
        u = u + u_offset;
        v = v + v_offset;

        // Wrap UV into [0,1)
        u = u - floor(u);
        v = v - floor(v);

        // Flip V for image coordinates
        v = 1.0 - v;

        // Convert to pixel space
        int i = static_cast<int>(u * image.width());
        int j = static_cast<int>(v * image.height());

        // Clamp to valid pixel indices
        i = std::min(i, image.width() - 1);
        j = std::min(j, image.height() - 1);

        auto pixel = image.pixel_data(i, j);

        const double color_scale = 1.0 / 255.0;
        return color(color_scale * pixel[0],
                     color_scale * pixel[1],
                     color_scale * pixel[2]);
    }

private:
    rtw_image image;
    double u_offset;
    double v_offset;
};

class noise_texture : public texture
{
public:
    noise_texture(double scale) : scale(scale) {}

    color value(double u, double v, const point3 &p) const override
    {
        return color(.5, .5, .5) * (1 + std::sin(scale * p.z + 10 * noise.turb(p, 7)));
    }

private:
    perlin noise;
    double scale;
};

#endif