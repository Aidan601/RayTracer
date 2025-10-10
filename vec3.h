#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

class vec3
{
public:
    double x;
    double y;
    double z;

    vec3()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    vec3(double x1, double y1, double z1)
    {
        x = x1;
        y = y1;
        z = z1;
    }

    vec3 operator+(const vec3 &v) const
    {
        return vec3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    vec3 operator+(double d) const
    {
        return vec3(this->x + d, this->y + d, this->z + d);
    }

    vec3 operator+=(const vec3 &v) const
    {
        return vec3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    vec3 operator-(const vec3 &v) const
    {
        return vec3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    vec3 operator-(double d) const
    {
        return vec3(this->x - d, this->y - d, this->z - d);
    }

    vec3 operator-=(const vec3 &v)
    {
        return vec3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    vec3 operator*(const vec3 &v) const
    {
        return vec3(this->x * v.x, this->y * v.y, this->z * v.z);
    }

    vec3 operator*(double d) const
    {
        return vec3(this->x * d, this->y * d, this->z * d);
    }

    friend vec3 operator*(double d, const vec3 &v)
    {
        return vec3(v.x * d, v.y * d, v.z * d);
    }

    vec3 operator/(const vec3 &v)
    {
        return vec3(this->x / v.x, this->y / v.y, this->z / v.z);
    }

    vec3 operator/(double d) const
    {
        return vec3(this->x / d, this->y / d, this->z / d);
    }

    bool operator==(const vec3 &v)
    {
        if (this->x == v.x && this->y == v.y && this->z == v.z)
            return true;
        else
            return false;
    }

    bool operator!=(const vec3 &v)
    {
        if (this->x == v.x && this->y == v.y && this->z == v.z)
            return false;
        else
            return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const vec3 &v)
    {
        os << v.x << " " << v.y << " " << v.z;
        return os;
    }

    double length() const
    {
        return (sqrt(x * x + y * y + z * z));
    }

    bool contains(const double &n)
    {
        if (x == n || y == n || z == n)
            return true;
        else
            return false;
    }
};

using point3 = vec3;

inline vec3 unit_vector(const vec3 &v)
{
    return v / v.length();
}

inline double dot(const vec3 &v1, const vec3 &v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

#endif