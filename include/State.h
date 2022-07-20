#ifndef STATE_H
#define STATE_H

#include <vector>
#include <string>
#include <memory>
#include "GameObject.h"

class State {
    public:
        State();
        virtual ~State();

        virtual void LoadAssets() = 0;
        virtual void Update(float dt) = 0;
        virtual void Render() = 0;

        virtual void Start() = 0;
        virtual void Pause() = 0;
        virtual void Resume() = 0;

        virtual std::weak_ptr<GameObject> AddObject(GameObject* object);
        virtual std::weak_ptr<GameObject> GetObjectPtr(GameObject* object);
        /**
         * @brief return a vector of gameobjects that contain some component
         * 
         * @param component 
         * @return std::vector<std::weak_ptr<GameObject>> 
         */
        virtual std::vector<std::weak_ptr<GameObject>> QueryObjectsBy(std::string component);

        bool PopRequested();
        bool QuitRequested();
    
    protected:
        void StartArray();
        virtual void UpdateArray(float dt);
        virtual void RenderArray();

        bool popRequested;
        bool quitRequested;
        bool started;

        std::vector<std::shared_ptr<GameObject>> objectArray;
};
#endif