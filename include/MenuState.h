#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "State.h"

class MenuState : public State
{
public:
    MenuState();
    ~MenuState();

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