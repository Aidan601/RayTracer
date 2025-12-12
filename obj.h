#ifndef OBJ_H
#define OBJ_H

#include "vec3.h"
#include "ray.h"
#include "hittable.h"

class obj : public hittable
{
public:
    obj() {

    };

private:
    std::vector<triangle> triangles;
};

class triangle
{
public:
    tri_vert v1;
    tri_vert v2;
    tri_vert v3;

    triangle()
    {
        v1 = tri_vert(point3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0));
        v2 = tri_vert(point3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0));
        v3 = tri_vert(point3(0, 0, 0), vec3(0, 0, 0), vec3(0, 0, 0));
    }

    triangle(tri_vert &first, tri_vert &second, tri_vert &third)
    {
        v1 = first;
        v2 = second;
        v3 = third;
    }
};

class tri_vert
{
public:
    point3 vertex;
    vec3 texture;
    vec3 normal;

    tri_vert()
    {
        vertex = point3(0, 0, 0);
        texture = vec3(0, 0, 0);
        normal = vec3(0, 0, 0);
    }

    tri_vert(point3 &point, vec3 &norm)
    {
        vertex = point;
        texture = vec3(0, 0, 0);
        normal = norm;
    }

    tri_vert(point3 &point, vec3 &text, vec3 &norm)
    {
        vertex = point;
        texture = text;
        normal = norm;
    }
};

#endif
