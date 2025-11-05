#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

#include <iostream>

class sphere
{
public:
    sphere(point3 centerInput, double radiusInput)
    {
        center = centerInput;
        radius = radiusInput;
    }

    bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const
    {
        double t;
        auto a = dot(r.direction, r.direction);
        auto h = dot(r.direction, center - r.origin);
        auto c = dot((center - r.origin), (center - r.origin)) - (radius * radius);
        double discriminant = (h * h) - (a * c);
        if (discriminant < 0) // 0 ROOTS
            return false;
        else // 1 or 2 ROOTS (We only calculate the one the camera sees for now)
            t = (h - sqrt(discriminant)) / a;

        rec.t = t;
        return true;
    }

private:
    point3 center;
    double radius;
};

#endif