#ifndef HITTABLE_H
#define HITTABLE_H

#include "raytracer.h"
#include "aabb.h"

class material;

class hit_record
{
public:
    point3 p;        // point that will be hit
    vec3 normal;     // vertex normal
    double t;        // t
    double u;        // u
    double v;        // v
    bool front_face; // true if it is front facing
    shared_ptr<material> mat;

    void set_face_normal(const ray &r, const vec3 &outward_normal) // Sets the hit record normal vector.
    {
        front_face = dot(r.direction, outward_normal) < 0;
        if (front_face)
            normal = outward_normal;
        else
            normal = -outward_normal;
    }
};

class hittable
{
public:
    virtual ~hittable() = default;
    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;
    virtual aabb bounding_box() const = 0;
};

#endif