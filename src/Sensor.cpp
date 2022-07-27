#include "Sensor.h"
#include "Game.h"
#include "StageState.h"
#include "Camera.h"

Sensor::Sensor(GameObject& associated, int destiny) : Component(associated), destiny(destiny){

}

void Sensor::Update(float dt){

}

void Sensor::Render(){

}

bool Sensor::Is(std::string type){
    if(type.compare("Sensor") == 0){
        return true;
    }

    return false;
}

void Sensor::NotifyCollision(GameObject& other){
    if(other.GetComponent("Player") != nullptr){
        Game::GetInstance().GetCurrentState().PopRequested();
        Game::GetInstance().Push(new StageState(destiny));
        Camera::Reset();
    }
}