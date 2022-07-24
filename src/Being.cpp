#include "Being.h"

Being::Being(GameObject& associated, Vec2 speed, float mass, int hp, int dir) : Component(associated), speed(speed), mass(mass), hp(hp), charState(IDLE), dir(dir){

}

Being::~Being(){

}