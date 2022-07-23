#include "Collider.h"
#include "Game.h"
#include "Camera.h"

Collider::Collider(GameObject& associated, Vec2 scale, Vec2 offset) : Component(associated), scale(scale), offset(offset) {
	
}

void Collider::Update(float dt){
	this->box = Rect(associated.box.x, associated.box.y, 32*scale.x, 64*scale.y);
    this->box.Centered(associated.box.Center());
	this->box = this->box + Vec2::Rotate(offset, associated.angleDeg*PI/180);


}

void Collider::Render(){
#ifdef DEBUG
	Vec2 center( box.Center() );
	SDL_Point points[5];

	Vec2 point = Vec2::Rotate(Vec2(box.x, box.y) - center, associated.angleDeg/(180/PI ))
					+ center - Camera::pos;
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};
	
	point = Vec2::Rotate(Vec2(box.x+box.w, box.y) - center, associated.angleDeg/(180/PI ))
					+ center - Camera::pos;
	points[1] = {(int)point.x, (int)point.y};
	
	point = Vec2::Rotate(Vec2(box.x+box.w, box.y+box.h) - center, associated.angleDeg/(180/PI ))
					+ center - Camera::pos;
	points[2] = {(int)point.x, (int)point.y};
	
	point = Vec2::Rotate(Vec2(box.x, box.y+box.h) - center, associated.angleDeg/(180/PI ))
					+ center - Camera::pos;
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

void Collider::NotifyCollision(GameObject& other){}

bool Collider::Is(std::string type){
    if(type.compare("Collider") == 0){
        return true;
    }

    return false;
}

void Collider::SetScale(Vec2 scale) { this->scale = scale; }
void Collider::SetOffset(Vec2 offset) { this->offset = offset; }