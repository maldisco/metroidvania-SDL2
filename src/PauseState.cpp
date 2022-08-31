#include "PauseState.h"
#include "Sprite.h"
#include "Text.h"
#include "InputManager.h"
#include "StageState.h"
#include "Camera.h"
#include "Game.h"
#include "GameData.h"
#include "Helpers.h"

#include <algorithm>

PauseState::PauseState() : State(), selectedOption(0), options({"Continue", "Exit"})
{
    SDL_Color color = {0, 0, 0, SDL_ALPHA_OPAQUE};
    Vec2 pos = {CAMERA_WIDTH / 2, CAMERA_HEIGHT / 2};
    GameObject *text;
    // Writing options
    for (int i = 0; i < options.size(); i++)
    {
        text = new GameObject(pos.x, pos.y + (i * 50));
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
    PaintOptions();
    HandleInput();
}

void PauseState::HandleInput()
{
    int up = InputManager::GetInstance().KeyPress(UP_ARROW_KEY) ? 1 : 0;
    int down = InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY) ? 1 : 0;
    int select = InputManager::GetInstance().KeyPress(E_KEY) ? 1 : 0;
    quitRequested = InputManager::GetInstance().QuitRequested();
    popRequested = InputManager::GetInstance().KeyPress(ESCAPE_KEY);

    selectedOption = Helpers::Clamp(0, selectedOption + (down - up), (int)options.size()-1);
    if (select)
        SelectOption(selectedOption);
}

void PauseState::SelectOption(int option)
{
    switch(option)
    {
        case 0:
            popRequested = true;
            break;
        case 1:
            quitRequested = true;
            break;
        default:
            return;
    }
}

void PauseState::PaintOptions()
{
    int curOption = 0;
    for (auto go : QueryObjectsBy<Text>())
    {
        if (curOption == selectedOption)
        {
            static_cast<Text*>(go.lock()->GetComponent<Text>())->SetColor({255, 0, 0, SDL_ALPHA_OPAQUE});
        }
        else
        {
            static_cast<Text*>(go.lock()->GetComponent<Text>())->SetColor({255, 255, 255, SDL_ALPHA_OPAQUE});
        }
        curOption++;
    }
}

void PauseState::Render()
{
    // Render every object
    RenderArray();
}
