#ifndef PENGUINBODY_H
#define PENGUINBODY_H

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
        std::weak_ptr<GameObject> pcannon;
        Vec2 speed;
        float linearSpeed;
        float angle;
        int hp;
};
#endif