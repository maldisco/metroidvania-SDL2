#ifndef PENGUINCANNON_H
#define PENGUINCANNON_H

#include "Component.h"
#include "Timer.h"

class PenguinCannon : public Component{
    public:
        /**
         * @brief Construct a new Penguin Cannon object
         * 
         * @param associated 
         * @param penguinBody 
         */
        PenguinCannon(GameObject& associated, std::weak_ptr<GameObject> penguinBody);
        
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        void Shoot();
    private:
        std::weak_ptr<GameObject> pbody;
        float angle;
        Timer cooldown;
};
#endif