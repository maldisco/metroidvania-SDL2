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
        Being(GameObject& associated, Vec2 speed, float mass, int hp);

        virtual ~Being();
    
    protected:
        Vec2 speed;
        float mass;
        int hp;
        enum STATE{ IDLE, WALKING, JUMPING, FALLING, ATTACKING, HURT, DEAD };
        STATE charState;
};
#endif