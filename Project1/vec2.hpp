#pragma once
#include <math.h>
#include <SFML/Graphics.hpp>
#include <iostream>

template <typename T>
// Vec2 class for 2D vector operations
class Vec2
{
public:
    T x = 0;
    T y = 0;

    Vec2() = default;
    Vec2(T xin, T yin)
        : x(xin), y(yin) {
    }

    //constructor to convert from sf::vector2
    Vec2(const sf::Vector2<T>& vec)
        : x(vec.x), y(vec.y) {
    }

    //this lets up pass vec2 into sfml functions
    operator sf::Vector2<T>()
    {
        return sf::Vector2<T>(x, y);
    }
    // Add vector to the current vector
    Vec2& add(const Vec2& v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    // Scale the vector by a scalar value
    Vec2& scale(float scale)
    {
        x *= scale;
        y *= scale;
        return *this;
    }

    // Overload operator + to add two vectors
    Vec2 operator + (const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 operator - (const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    Vec2 operator / (const T val) const
    {
        return Vec2(x / val, y / val);
    }
    // Overload operator * to scale the vector
    Vec2 operator * (const T val) const
    {
        return Vec2(x * val, y * val);
    }

    // Calculate the distance (magnitude/length) of the vector
    float length() const
    {
        return std::sqrtf(x * x + y * y);
    }

    float dist(const Vec2& rhs) const
    {
        return sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
    }
    // Overload equality operator to compare two vectors
    bool operator==(const Vec2& rhs) const
    {
        return (x == rhs.x && y == rhs.y);
    }

    // Overload inequality operator
    bool operator!=(const Vec2& rhs) const
    {
        return !(*this == rhs);
    }

    void operator += (const Vec2& rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
    }

    void operator -= (const Vec2& rhs)
    {
        x = x - rhs.x;
        y = y - rhs.y;
    }

    void operator *= (const T val)
    {
        x = x * val;
        y = y * val;
    }

    void operator /= (const T val)
    {
        x = x / val;
        y = y / val;
    }
};

using Vec2f = Vec2<float>;

