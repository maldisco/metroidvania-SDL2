#ifndef VEC2_H
#define VEC2_H

#include <string>
#include "math.h"
#define PI 3.14159265

/**
 * @brief represents both a point and a vector
 *
 */
class Vec2
{
public:
    float x, y;

    Vec2();
    Vec2(float x, float y);

    Vec2 operator+(const Vec2 &rhs) const;
    Vec2 operator-(const Vec2 &rhs) const;
    Vec2 operator*(const float rhs) const;

    /**
     * @brief the distance between two vectors
     *
     * @param v1
     * @param v2
     * @return float
     */
    static float Distance(Vec2 v1, Vec2 v2);

    /**
     * @brief slope of a line from 2 points
     *
     * @return float
     */
    static float Slope(Vec2 v1, Vec2 v2);

    /**
     * @brief slope of the vector on x (in radians)
     *
     * @return float
     */
    float SlopeX();

    /**
     * @brief Return magnitude of vector
     *
     * @param vec
     * @return float
     */
    static inline float Magnitude(const Vec2 &vec)
    {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    /**
     * @brief Return normalized vector (magnitude = 1)
     *
     * @param vec
     * @return Vec2
     */
    static inline Vec2 Normalize(const Vec2 &vec)
    {
        return vec * (1.f / Magnitude(vec));
    }

    /**
     * @brief Dot product of 2 vectors
     *
     * @param v1
     * @param v2
     * @return float
     */
    static inline float Dot(const Vec2 &v1, const Vec2 &v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    /**
     * @brief Rotate the vector by some angle
     *
     * @param vec
     * @param angle
     * @return Vec2
     */
    static inline Vec2 Rotate(const Vec2 &vec, float angle)
    {
        return Vec2(vec.x * cos(angle) - vec.y * sin(angle), vec.y * cos(angle) + vec.x * sin(angle));
    }
};
#endif