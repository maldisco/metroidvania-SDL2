#include "Vec2.h"
#include "Rect.h"
#include "math.h"

Vec2::Vec2() : x(0), y(0) {}
Vec2::Vec2(float x, float y) : x(x), y(y) {}

Vec2 Vec2::operator+(const Vec2 &rhs) const
{
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2 &rhs) const
{
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float rhs) const
{
    return Vec2(x * rhs, y * rhs);
}

float Vec2::Distance(Vec2 v1, Vec2 v2)
{
    return Magnitude(v1 - v2);
}

float Vec2::SlopeX()
{
    return atan2(y, x);
}

float Vec2::Slope(Vec2 v1, Vec2 v2)
{
    return atan2(v1.y, v1.x) - atan2(v2.y, v2.x);
}
