#ifndef BULLET_H
#define BULLET_H
#include "Component.h"

/**
 * @brief Object alien shoots
 * 
 */
class Bullet : public Component{
    public:
        /**
         * @brief Construct a new Bullet object
         * 
         * @param associated 
         * @param angle 
         * @param speed 
         * @param damage 
         * @param maxDistance 
         * @param sprite 
         * @param frameTime 
         * @param frameCount 
         * @param targetsPlayer 
         */
        Bullet(GameObject& associated, float angle, float speed, int damage, float maxDistance, std::string sprite, float frameTime, int frameCount, bool targetsPlayer);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        int GetDamage();
        void NotifyCollision(GameObject& other);

        bool targetsPlayer;
    private:
        Vec2 speed;
        float distanceLeft;
        int damage;
};
#endif