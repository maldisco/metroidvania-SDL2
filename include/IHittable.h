#ifndef IHITTABLE_H
#define IHITTABLE_H

#include "Rect.h"

class IHittable
{
public:
    virtual void HandleDamage(Rect &box) = 0;
};

#endif