#ifndef DAMAGE_H
#define DAMAGE_H
#include "Component.h"
#include "Timer.h"

/**
 * @brief Damage box
 * 
 */
class Damage : public Component{
    public:
        /**
         * @brief Construct a new Damage object
         * 
         * @param associated 
         * @param damage 
         * @param targetsPlayer 
         */
        Damage(GameObject& associated, int damage, bool targetsPlayer, float msToSelfDestruct = 0.1f);
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        int GetDamage();
        void NotifyCollision(GameObject& other);

        bool targetsPlayer;
    private:
        Timer lifeTime;
        int damage;
        float msToSelfDestruct;
};
#endif