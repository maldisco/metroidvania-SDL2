#ifndef ALIEN_H
#define ALIEN_H

#include "Component.h"
#include "Timer.h"
#include <queue>


/**
 * @brief Main enemy
 * 
 */
class Alien : public Component{
    public:
        /**
         * @brief Construct a new Alien object
         * 
         * @param associated 
         */
        Alien(GameObject& associated, float timeOffset = 0);
        ~Alien();

        void Start();
        void Update(float dt);
        void Render();
        void NotifyCollision(GameObject& other);
        bool Is(std::string type);

        static int alienCount;
    private:
        enum AlienState{ MOVING, RESTING };
        AlienState state;
        Timer restTimer;
        float timeOffset;
        Vec2 destination;
        Vec2 speed;
        int hp;

        std::vector<std::weak_ptr<GameObject>> minionArray;
};
#endif