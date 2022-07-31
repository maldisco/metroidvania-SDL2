#include "PauseState.h"
#include "Sprite.h"
#include "Text.h"
#include "InputManager.h"
#include "StageState.h"
#include "Camera.h"
#include "Game.h"
#include "GameData.h"

#include <algorithm>

PauseState::PauseState() : State(), selectedOption(0), options({"Continue", "Exit"})
{
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

PauseState::~PauseState()
{
    objectArray.clear();
    Camera::pos = GameData::cameraPos;
}

void PauseState::LoadAssets()
{
}

void PauseState::Pause()
{
}

void PauseState::Resume()
{
}

void PauseState::Start()
{
    GameData::cameraPos = Camera::pos;
    Camera::Reset();
    LoadAssets();
    StartArray();
    started = true;
}

void PauseState::Update(float dt)
{
    UpdateArray(dt);

    // Paint the selected option red
    int curOption = 0;
    for (auto go : objectArray)
    {
        if (go->GetComponent("Text") != nullptr)
        {
            if (curOption == selectedOption)
            {
                ((Text *)go->GetComponent("Text"))->SetColor({255, 0, 0, SDL_ALPHA_OPAQUE});
            }
            else
            {
                ((Text *)go->GetComponent("Text"))->SetColor({255, 255, 255, SDL_ALPHA_OPAQUE});
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
            popRequested = true;
        else if (selectedOption == 1)
            quitRequested = true;

    // check if quit was requested
    if (InputManager::GetInstance().KeyPress(ESCAPE_KEY) || InputManager::GetInstance().QuitRequested())
    {
        quitRequested = true;
    }
}

void PauseState::Render()
{
    // Render every object
    RenderArray();
}
