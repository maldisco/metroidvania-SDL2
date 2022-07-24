#ifndef BEING_H
#define BEING_H
#include "Component.h"

class Being : public Component{
    public:
        /**
         * @brief Construct a new Being object
         * 
         * @param associated 
         * @param speed 
         * @param mass 
         * @param hp 
         */
        Being(GameObject& associated, Vec2 speed, float mass, int hp, int dir = 1);

        virtual ~Being();
    
    protected:
        Vec2 speed;
        float mass;
        int hp;
        int dir;
        enum STATE{ IDLE, WALKING, JUMPING, FALLING, ATTACKING, DASHING, HURT, DEAD };
        STATE charState;
};
#endif