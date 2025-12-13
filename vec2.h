#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include <cmath>
#include <stdexcept>

double random_double();
double random_double(double min, double max);

class vec2
{
public:
    double x;
    double y;

    vec2()
    {
        x = 0;
        y = 0;
    }

    vec2(double x1, double y1)
    {
        x = x1;
        y = y1;
    }

    double operator[](char c) const
    {
        switch (c)
        {
        case 'x':
            return x;
        case 'y':
            return y;
        }
        throw std::out_of_range("Invalid vec2 index");
    }

    double operator[](int i) const
    {
        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        }
        throw std::out_of_range("Invalid vec2 index");
    }

    double &operator[](int i)
    {
        switch (i)
        {
        case 0:
            return x;
        case 1:
            return y;
        }
        throw std::out_of_range("Invalid vec2 index");
    }

    vec2 operator+(const vec2 &v) const
    {
        return vec2(this->x + v.x, this->y + v.y);
    }

    vec2 operator+(double d) const
    {
        return vec2(this->x + d, this->y + d);
    }

    vec2 &operator+=(const vec2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    vec2 operator-(const vec2 &v) const
    {
        return vec2(this->x - v.x, this->y - v.y);
    }

    vec2 operator-(double d) const
    {
        return vec2(this->x - d, this->y - d);
    }

    vec2 &operator-=(const vec2 &v)
    {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vec2 &operator*=(const int n)
    {
        x = x * n;
        y = y * n;
        return *this;
    }

    vec2 operator*(const vec2 &v) const
    {
        return vec2(this->x * v.x, this->y * v.y);
    }

    vec2 operator*(double d) const
    {
        return vec2(this->x * d, this->y * d);
    }

    friend vec2 operator*(double d, const vec2 &v)
    {
        return vec2(v.x * d, v.y * d);
    }

    vec2 operator/(const vec2 &v)
    {
        return vec2(this->x / v.x, this->y / v.y);
    }

    vec2 operator-() const
    {
        return vec2(-this->x, -this->y);
    }

    vec2 operator/(double d) const
    {
        return vec2(this->x / d, this->y / d);
    }

    bool operator==(const vec2 &v)
    {
        if (this->x == v.x && this->y == v.y)
            return true;
        else
            return false;
    }

    bool operator!=(const vec2 &v)
    {
        if (this->x == v.x && this->y == v.y)
            return false;
        else
            return true;
    }

    friend std::ostream &operator<<(std::ostream &os, const vec2 &v)
    {
        os << v.x << " " << v.y;
        return os;
    }

    double length() const
    {
        return (sqrt(x * x + y * y));
    }

    double length_squared() const
    {
        return x * x + y * y;
    }

    bool contains(const double &n)
    {
        if (x == n || y == n)
            return true;
        else
            return false;
    }

    static vec2 random()
    {
        return vec2(random_double(), random_double());
    }

    static vec2 random(double min, double max)
    {
        return vec2(random_double(min, max), random_double(min, max));
    }
};

#endif