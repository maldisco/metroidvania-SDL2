#ifndef PHYSICS_H
#define PHYSICS_H
#include "Rect.h"
#include "Vec2.h"
#include "Game.h"
#include "Helpers.h"
#include "Collider.h"

#include <algorithm>
#include <cmath>

class Physics
{
public:
    /**
     * @brief Check for collisions within a circle
     *
     * @param center
     * @param radius
     * @param layer
     * @return std::vector<Collider *>
     */
    static std::vector<Collider *> OverlapCircleAll(Vec2 center, float radius, Enums::Layer layer)
    {
        std::vector<Collider *> ret;
        for (auto go : Game::GetInstance().GetCurrentState().QueryObjectsBy<Collider>())
        {
            if (go.lock()->layer == layer)
            {
                auto collider = static_cast<Collider *>(go.lock()->GetComponent<Collider>());
                auto b = collider->box;
                auto angleOfB = go.lock()->angleDeg;
                if (CircleRectCollision(center, radius, b, angleOfB))
                {
                    ret.push_back(collider);
                }
            }
        }
        return ret;
    }

    /**
     * @brief Check collision between a circle and a rectangle
     *
     * @param center Center of circle
     * @param radius Radius of circle
     * @param rect Rectangle
     * @param angleOfRect Angle of the rectangle
     * @return true
     * @return false
     */
    static bool CircleRectCollision(Vec2 center, float radius, Rect &rect, float angleOfRect)
    {
        Vec2 rotatedCircle = Vec2(cos(angleOfRect) * (center.x - rect.Center().x) - sin(angleOfRect) * (center.y - rect.Center().y) + rect.Center().x,
                                  sin(angleOfRect) * (center.x - rect.Center().x) + cos(angleOfRect) * (center.y - rect.Center().y) + rect.Center().y);

        Vec2 nearest = Vec2(Helpers::Clamp(rect.x, rotatedCircle.x, rect.x + rect.w), Helpers::Clamp(rect.y, rotatedCircle.y, rect.y + rect.h));
        return Vec2::Magnitude(rotatedCircle - nearest) <= radius;
    }

    /**
     * @brief Check for collision between two rectangles (a and b)
     *
     * @param a
     * @param b
     * @param angleOfA
     * @param angleOfB
     * @return true
     * @return false
     */
    static bool IsColliding(Rect &a, Rect &b, float angleOfA, float angleOfB)
    {
        Vec2 A[] = {Vec2(a.x, a.y + a.h),
                    Vec2(a.x + a.w, a.y + a.h),
                    Vec2(a.x + a.w, a.y),
                    Vec2(a.x, a.y)};
        Vec2 B[] = {Vec2(b.x, b.y + b.h),
                    Vec2(b.x + b.w, b.y + b.h),
                    Vec2(b.x + b.w, b.y),
                    Vec2(b.x, b.y)};

        for (auto &v : A)
        {
            v = Vec2::Rotate(v - a.Center(), angleOfA) + a.Center();
        }

        for (auto &v : B)
        {
            v = Vec2::Rotate(v - b.Center(), angleOfB) + b.Center();
        }

        Vec2 axes[] = {Vec2::Normalize(A[0] - A[1]), Vec2::Normalize(A[1] - A[2]), Vec2::Normalize(B[0] - B[1]), Vec2::Normalize(B[1] - B[2])};

        for (auto &axis : axes)
        {
            float P[4];

            for (int i = 0; i < 4; ++i)
                P[i] = Vec2::Dot(A[i], axis);

            float minA = *std::min_element(P, P + 4);
            float maxA = *std::max_element(P, P + 4);

            for (int i = 0; i < 4; ++i)
                P[i] = Vec2::Dot(B[i], axis);

            float minB = *std::min_element(P, P + 4);
            float maxB = *std::max_element(P, P + 4);

            if (maxA < minB || minA > maxB)
                return false;
        }

        return true;
    }

    static void DrawCircle(SDL_Renderer *renderer, int32_t centreX, int32_t centreY, int32_t radius)
    {
        const int32_t diameter = (radius * 2);

        int32_t x = (radius - 1);
        int32_t y = 0;
        int32_t tx = 1;
        int32_t ty = 1;
        int32_t error = (tx - diameter);

        while (x >= y)
        {
            //  Each of the following renders an octant of the circle
            SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
            SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
            SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
            SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
            SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
            SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

            if (error <= 0)
            {
                ++y;
                error += ty;
                ty += 2;
            }

            if (error > 0)
            {
                --x;
                tx += 2;
                error += (tx - diameter);
            }
        }
    }
};
#endif