#include "InputManager.h"

#define INCLUDE_SDL
#include "SDL_include.h"

InputManager& InputManager::GetInstance(){
    static InputManager inputManager;
    return inputManager;
}

InputManager::InputManager() : mouseState{false}, mouseUpdate{0}, quitRequested(false), updateCounter(0), mouseX(0), mouseY(0){}

InputManager::~InputManager(){}

void InputManager::Update(){
    SDL_Event event;

    SDL_GetMouseState(&mouseX, &mouseY);
    quitRequested = false;
    updateCounter++;

    while(SDL_PollEvent(&event)){

        if(event.type == SDL_KEYDOWN){
            if(event.key.repeat != 1){
                keyState[event.key.keysym.sym] = true;
                keyUpdate[event.key.keysym.sym] = updateCounter;
            }
        } else if (event.type == SDL_KEYUP){
            keyState[event.key.keysym.sym] = false;
            keyUpdate[event.key.keysym.sym] = updateCounter;
        } else if (event.type == SDL_MOUSEBUTTONDOWN){
            mouseState[event.button.button] = true;
            mouseUpdate[event.button.button] = updateCounter;
        } else if (event.type == SDL_MOUSEBUTTONUP){
            mouseState[event.button.button] = false;
            mouseUpdate[event.button.button] = updateCounter;
        } else if (event.type == SDL_QUIT){
            quitRequested = true;
        }

    }

}

bool InputManager::KeyPress(int key){
    return keyState[key] == true && keyUpdate[key] == updateCounter;
}

bool InputManager::KeyRelease(int key){
    return keyState[key] == false && keyUpdate[key] == updateCounter;
}

bool InputManager::IsKeyDown(int key){
    return keyState[key];
}

bool InputManager::MousePress(int button){
    return mouseState[button] == true && mouseUpdate[button] == updateCounter;
}

bool InputManager::MouseRelease(int button){
    return mouseState[button] == false && mouseUpdate[button] == updateCounter;
}

bool InputManager::IsMouseDown(int button){
    return mouseState[button];
}

int InputManager::GetMouseX(){ return mouseX; }

int InputManager::GetMouseY(){ return mouseY; }

bool InputManager::QuitRequested(){ return quitRequested; }