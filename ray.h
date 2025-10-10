#ifndef RAY_H
#define RAY_H

#include <iostream>
#include "vec3.h"

class ray
{
public:
    vec3 origin;
    vec3 direction;

    ray()
    {
        origin = vec3(0, 0, 0);
        direction = vec3(0, 0, 0);
    }

    ray(vec3 vecA, vec3 vecB)
    {
        origin = vecA;
        direction = vecB;
    }

    vec3 at(double t) const
    {
        return origin + (direction * t);
    }
};

#endif