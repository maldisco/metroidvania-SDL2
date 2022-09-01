#ifndef TRANSITION_H
#define TRANSITION_H

#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

#include "Component.h"
#include "Timer.h"
#include "State.h"

class Transition : public Component
{
public:
    static Transition *Instance;

    Transition(GameObject &associated, float duration);
    ~Transition();

    void Start();
    void Update(float dt);
    void Render();

    void FadeIn(float dt);
    void FadeOut(float dt);

    void StartTransition(State *stage);
private:
    std::shared_ptr<SDL_Texture> texture;
    State *stage;

    Timer transitionTime;
    int alpha;
    float transitionDuration;
    bool fadeIn, fadeOut;
};

#endif