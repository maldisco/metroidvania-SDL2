#ifndef PENGUINBODY_H
#define PENGUINBODY_H

#define MAX_SPEEDH 200
#define JUMP_FORCE 15000

#include "Component.h"

class Android : public Component {
    public:
        /**
         * @brief Construct a new Penguin Body object
         * 
         * @param associated 
         */
        Android(GameObject& associated);
        ~Android();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        static Android* player;
    private:
        Vec2 speed;
        float mass;
        int hp;
        // player state
        enum STATE{ STANDING, WALKING, JUMPING, FALLING };
        STATE playerState;
};
#endif