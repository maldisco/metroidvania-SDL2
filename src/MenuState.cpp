#include "MenuState.h"
#include "Sprite.h"
#include "Text.h"
#include "InputManager.h"
#include "StageState.h"
#include "Camera.h"
#include "Game.h"

#include <algorithm>

MenuState::MenuState() : State(), selectedOption(0), options({"Start", "Exit"})
{
    GameObject *bg = new GameObject();
    bg->AddComponent(new Sprite("assets/img/background.png", *bg, 1, 1.0f));
    bg->box.x = 0;
    bg->box.y = 0;
    AddObject(bg);

    SDL_Color color = {0, 0, 0, SDL_ALPHA_OPAQUE};
    Vec2 pos = {CAMERA_WIDTH / 2, CAMERA_HEIGHT / 2};
    GameObject *text;
    // Writing options
    for (int i = 0; i < options.size(); i++)
    {
        text = new GameObject();
        text->box.x = pos.x;
        text->box.y = pos.y + (i * 50);
        text->AddComponent(new Text(*text, PEABERRY_FONT, 50, Text::BLENDED, options[i], color));
        
        AddObject(text);
    }
}

MenuState::~MenuState()
{
    objectArray.clear();
}

void MenuState::LoadAssets() {}

void MenuState::Pause() {}

void MenuState::Resume()
{
    Camera::Reset();
}

void MenuState::Start()
{
    Camera::Reset();
    LoadAssets();
    StartArray();
    started = true;
}

void MenuState::Update(float dt)
{
    UpdateArray(dt);

    // Paint the selected option red
    int curOption = 0;
    for(auto go : objectArray)
    {
        if(go->GetComponent("Text") != nullptr)
        {
            if(curOption == selectedOption)
            {
                ((Text*)go->GetComponent("Text"))->SetColor({255, 0, 0, SDL_ALPHA_OPAQUE});
            }
            else
            {
                ((Text*)go->GetComponent("Text"))->SetColor({255, 255, 255, SDL_ALPHA_OPAQUE});
            }
            curOption++;
        }
    }

    int up = InputManager::GetInstance().KeyPress(UP_ARROW_KEY) ? 1 : 0;
    int down = InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY) ? 1 : 0;
    int select = InputManager::GetInstance().KeyPress(E_KEY) ? 1 : 0;

    if (up)
        selectedOption = std::max(0, selectedOption - 1);
    else if (down)
        selectedOption = std::min((int)options.size() - 1, selectedOption + 1);
    else if (select)
        if (selectedOption == 0)
            Game::GetInstance().Push(new StageState(0));
        else if (selectedOption == 1)
            quitRequested = true;

    // check if quit was requested
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) || InputManager::GetInstance().QuitRequested())
    {
        quitRequested = true;
    }
}

void MenuState::Render()
{
    // Render every object
    RenderArray();
}
