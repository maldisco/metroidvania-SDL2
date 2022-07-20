#ifndef CAMERAFOLLOWER_H
#define CAMERAFOLLOWER_H

#include "Component.h"

/**
 * @brief Flags a component. Follow the camera.
 * 
 */
class CameraFollower : public Component {
    public:
        /**
         * @brief Construct a new Camera Follower object
         * 
         * @param associated 
         */
        CameraFollower(GameObject& associated);

        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);
};

#endif

