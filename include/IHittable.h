#ifndef IHITTABLE_H
#define IHITTABLE_H

#include "Rect.h"

/**
 * @brief Interface for signing that object can take damage
 * 
 */
class IHittable
{
public:
    /**
     * @brief Damage handler
     * 
     * @param box position of the origin of the damage
     */
    virtual void HandleDamage(Rect &box) = 0;
};

#endif