#ifndef PENGUINBODY_H
#define PENGUINBODY_H

#include "Component.h"

class PenguinBody : public Component {
    public:
        /**
         * @brief Construct a new Penguin Body object
         * 
         * @param associated 
         */
        PenguinBody(GameObject& associated);
        ~PenguinBody();

        void Start();
        void Update(float dt);
        void Render();
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        static PenguinBody* player;
    private:
        std::weak_ptr<GameObject> pcannon;
        Vec2 speed;
        float linearSpeed;
        float angle;
        int hp;
};
#endif