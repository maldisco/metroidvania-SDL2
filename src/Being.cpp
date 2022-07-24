#include "Being.h"

Being::Being(GameObject& associated, Vec2 speed, float mass, int hp) : Component(associated), speed(speed), mass(mass), hp(hp), charState(IDLE){

}

Being::~Being(){
    
}