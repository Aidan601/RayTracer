#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class sphere : public hittable
{
public:
    sphere(const point3 &centerInput, double radiusInput)
        : center(centerInput), radius(radiusInput) {}

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        // Ray-sphere intersection using the simplified quadratic
        vec3 oc = center - r.origin;
        auto a = r.direction.length_squared();
        auto h = dot(r.direction, oc);
        auto c = oc.length_squared() - radius * radius;
        auto discriminant = h * h - a * c;

        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root))
        {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);

        // Outward normal and front-face logic
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

private:
    point3 center;
    double radius;
};

#endif
