#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"

/**
 * @brief A transform component has his position relative to associated game object
 * 
 */
class Transform : public Component
{
public:
    Vec2 position;

    /**
     * @brief Construct a new Transform object
     * 
     * @param associated 
     * @param offset 
     */
    Transform(GameObject &associated, Vec2 offset);

    void Update(float dt);

private:
    Vec2 offset;
};

#endif