#ifndef RECT_H
#define RECT_H

#include "Vec2.h"

/**
 * @brief represents a rectangle 
 */
class Rect{
    public:
        float x, y, w, h;

        Rect();
        Rect(float x, float y, float w, float h);

        Rect operator+(const Vec2& rhs) const;

        /**
         * @brief returns the point in the center of the rectangle
         * 
         * @return Vec2 
         */
        Vec2 Center();

        /**
         * @brief center box in a given point
         * 
         * @param center 
         */
        void Centered(Vec2 center);

        /**
         * @brief distance between the center of two rectangles
         * 
         * @param r1 
         * @param r2 
         * @return float 
         */
        static float Distance(Rect r1, Rect r2);

        /**
         * @brief if a given point is inside rect
         * 
         * @param x 
         * @param y 
         * @return true 
         * @return false 
         */
        bool Contains( Vec2 v2 );
};
#endif