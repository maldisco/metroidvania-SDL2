#include "Collider.h"
#include "Game.h"
#include "TileMap.h"
#include "TileSet.h"
#include "StageState.h"
#include "Camera.h"

Collider::Collider(GameObject &associated, Vec2 scale, Vec2 offset) : Component(associated), scale(scale), offset(offset)
{
}

void Collider::Update(float dt)
{
	this->box = Rect(associated.box.x, associated.box.y, associated.box.w * scale.x, associated.box.h * scale.y);
	this->box.Centered(associated.box.Center());
	this->box = this->box + Vec2::Rotate(offset, associated.angleDeg * PI / 180);
}

bool Collider::IsGrounded()
{
	TileMap *tileMap = static_cast<StageState&>(Game::GetInstance().GetCurrentState()).GetTileMap();
    TileSet *tileSet = static_cast<StageState&>(Game::GetInstance().GetCurrentState()).GetTileSet();

	int tileLeftX = box.x / tileSet->GetTileWidth();
    int tileRightX = (box.x + box.w) / tileSet->GetTileWidth();
    int tileBottomY = (box.y + box.h + 1) / tileSet->GetTileHeight();
    return (tileMap->IsSolid(tileLeftX, tileBottomY) or tileMap->IsSolid(tileRightX, tileBottomY));
}

void Collider::Render()
{
#ifdef DEBUG
	Vec2 center(box.Center());
	SDL_Point points[5];

	Vec2 point = Vec2::Rotate(Vec2(box.x, box.y) - center, associated.angleDeg / (180 / PI)) + center - Camera::pos;
	points[0] = {(int)point.x, (int)point.y};
	points[4] = {(int)point.x, (int)point.y};

	point = Vec2::Rotate(Vec2(box.x + box.w, box.y) - center, associated.angleDeg / (180 / PI)) + center - Camera::pos;
	points[1] = {(int)point.x, (int)point.y};

	point = Vec2::Rotate(Vec2(box.x + box.w, box.y + box.h) - center, associated.angleDeg / (180 / PI)) + center - Camera::pos;
	points[2] = {(int)point.x, (int)point.y};

	point = Vec2::Rotate(Vec2(box.x, box.y + box.h) - center, associated.angleDeg / (180 / PI)) + center - Camera::pos;
	points[3] = {(int)point.x, (int)point.y};

	SDL_SetRenderDrawColor(Game::GetInstance().GetRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLines(Game::GetInstance().GetRenderer(), points, 5);
#endif // DEBUG
}

void Collider::SetScale(Vec2 scale) { this->scale = scale; }
void Collider::SetOffset(Vec2 offset) { this->offset = offset; }