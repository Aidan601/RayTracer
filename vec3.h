#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

double random_double();
double random_double(double min, double max);

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

    double operator[](char c) const
    {
        switch (c)
        {
        case 'x':
            return x;
        case 'y':
            return y;
        case 'z':
            return z;
        }
        throw std::out_of_range("Invalid vec3 index");
    }

    double operator[](int i) const
    {
        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        }
        throw std::out_of_range("Invalid vec3 index");
    }

    vec3 operator+(const vec3 &v) const
    {
        return vec3(this->x + v.x, this->y + v.y, this->z + v.z);
    }

    vec3 operator+(double d) const
    {
        return vec3(this->x + d, this->y + d, this->z + d);
    }

    vec3 &operator+=(const vec3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vec3 operator-(const vec3 &v) const
    {
        return vec3(this->x - v.x, this->y - v.y, this->z - v.z);
    }

    vec3 operator-(double d) const
    {
        return vec3(this->x - d, this->y - d, this->z - d);
    }

    vec3 &operator-=(const vec3 &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    vec3 &operator*=(const int n)
    {
        x = x * n;
        y = y * n;
        z = z * n;
        return *this;
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

    vec3 operator-() const
    {
        return vec3(-this->x, -this->y, -this->z);
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

    double length_squared() const
    {
        return x * x + y * y + z * z;
    }

    bool near_zero() const
    {
        const auto s = 1e-8;
        return (fabs(x) < s) && (fabs(y) < s) && (fabs(z) < s);
    }

    bool contains(const double &n)
    {
        if (x == n || y == n || z == n)
            return true;
        else
            return false;
    }

    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }
};

using point3 = vec3;

inline vec3 unit_vector(const vec3 &v)
{
    return v / v.length();
}

inline vec3 random_unit_vector()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        auto lensq = p.length_squared();
        if (lensq <= 1)
            return p / sqrt(lensq);
    }
}

// Dot product
inline double dot(const vec3 &v1, const vec3 &v2)
{
    return ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
}

inline vec3 random_on_hemisphere(const vec3 &normal)
{
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

// Cross product
inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.y * v2.z - v1.z * v2.y,
                v1.z * v2.x - v1.x * v2.z,
                v1.x * v2.y - v1.y * v2.x);
}

inline vec3 random_in_unit_disk()
{
    while (true)
    {
        auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

#endif