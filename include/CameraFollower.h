#ifndef CAMERAFOLLOWER_H
#define CAMERAFOLLOWER_H

#include "Component.h"

/**
 * @brief Flags a component. Follow the camera.
 *
 */
class CameraFollower : public Component
{
public:
    /**
     * @brief Construct a new Camera Follower object
     *
     * @param associated
     */
    CameraFollower(GameObject &associated, Vec2 offset = {0, 0});

    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(GameObject &other);

private:
    Vec2 offset;
};

#endif
