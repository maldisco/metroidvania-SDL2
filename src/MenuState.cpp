#include "MenuState.h"
#include "Sprite.h"
#include "Text.h"
#include "InputManager.h"
#include "StageState.h"
#include "Camera.h"
#include "Game.h"
#include "Helpers.h"

#include <algorithm>

MenuState::MenuState() : State(), selectedOption(0), options({"Start", "Exit"})
{
    GameObject *bg = new GameObject();
    bg->AddComponent(new Sprite("assets/img/background.png", *bg, 1, 1.0f));
    AddObject(bg);

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
    PaintOptions();
    HandleInput();    
}

void MenuState::HandleInput()
{
    int up = InputManager::GetInstance().KeyPress(UP_ARROW_KEY) ? 1 : 0;
    int down = InputManager::GetInstance().KeyPress(DOWN_ARROW_KEY) ? 1 : 0;
    int select = InputManager::GetInstance().KeyPress(E_KEY) ? 1 : 0;
    quitRequested = InputManager::GetInstance().KeyPress(ESCAPE_KEY) || InputManager::GetInstance().QuitRequested();

    selectedOption = Helpers::Clamp(0, selectedOption + (down - up), (int)options.size()-1);
    if (select)
        SelectOption(selectedOption);
}

void MenuState::SelectOption(int option)
{
    switch(option)
    {
        case 0:
            Game::GetInstance().Push(new StageState(0));
            break;
        case 1:
            quitRequested = true;
            break;
        default:
            return;
    }
}

void MenuState::PaintOptions()
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

void MenuState::Render()
{
    // Render every object
    RenderArray();
}
