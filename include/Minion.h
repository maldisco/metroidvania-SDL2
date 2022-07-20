#ifndef MINION_H
#define MINION_H

#include "Component.h"
#include "Sprite.h"

#define ARC PI/64

/**
 * @brief Entity linked to alien
 */
class Minion : public Component {
    public:
        /**
         * @brief Construct a new Minion object
         * 
         * @param associated 
         * @param alienCenter 
         * @param arcOffsetDeg 
         */
        Minion(GameObject& associated, std::weak_ptr<GameObject> alienCenter, float arcOffsetDeg = 0);

        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        void Shoot(Vec2 target);
    
    private:
        std::weak_ptr<GameObject> alienCenter;
        float arc;
};
#endif