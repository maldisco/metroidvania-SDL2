#ifndef PAUSESTATE_H
#define PAUSESTATE_H
#include "State.h"

class PauseState : public State
{
public:
    PauseState();
    ~PauseState();

    void LoadAssets();
    void Update(float dt);
    void Render();

    void Start();
    void Pause();
    void Resume();

private:
    int selectedOption;
    std::vector<std::string> options;
};
#endif