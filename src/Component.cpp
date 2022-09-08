#include "Component.h"

Component::Component(GameObject &associated) : associated(associated), enabled(true) {}

Component::~Component() {}

void Component::Start() {}

void Component::Update(float dt) {}

void Component::Render() {}

void Component::NotifyCollision(GameObject &other) {}