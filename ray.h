#ifndef RAY_H
#define RAY_H

class ray
{
public:
    vec3 origin;
    vec3 direction;
    double time;

    ray()
    {
        origin = vec3(0, 0, 0);
        direction = vec3(0, 0, 0);
        time = 0.0;
    }

    ray(vec3 vecA, vec3 vecB)
    {
        origin = vecA;
        direction = vecB;
        time = 0.0;
    }

    ray(vec3 vecA, vec3 vecB, double num)
    {
        origin = vecA;
        direction = vecB;
        time = num;
    }

    vec3 at(double t) const
    {
        return origin + (direction * t);
    }
};

#endif