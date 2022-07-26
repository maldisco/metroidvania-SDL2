#include "GameObject.h"
#include "algorithm"
#include "Component.h"

#include <typeinfo>

#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

GameObject::GameObject(float x, float y, Enums::Layer layer) : started(false), angleDeg(0), isDead(false), isActive(true), layer(layer), direction(1)
{
    box.x = x;
    box.y = y;
}

GameObject::~GameObject()
{
    components.clear();
}

void GameObject::Start()
{
    for (unsigned i = 0; i < components.size(); i++)
    {
        components[i]->Start();
    }
    started = true;
}

void GameObject::Update(float dt)
{
    for (unsigned i = 0; i < components.size(); i++)
    {
        if(components[i]->enabled)
            components[i]->Update(dt);
    }
}

void GameObject::Render()
{
    for (unsigned i = 0; i < components.size(); i++)
    {
        if(components[i]->enabled)
            components[i]->Render();
    }
}

bool GameObject::IsDead()
{
    return isDead;
}

void GameObject::SetActive(bool isActive)
{
    this->isActive = isActive;
}

void GameObject::RequestDelete()
{
    isDead = true;
}

void GameObject::AddComponent(Component *cpt)
{
    components.emplace_back(cpt);

    if (started)
    {
        cpt->Start();
    }
}

void GameObject::RemoveComponent(Component *cpt)
{
    for (unsigned i = 0; i < components.size(); i++)
    {
        if (components[i].get() == cpt)
        {
            components.erase(components.begin() + i);
        }
    }
}

void GameObject::NotifyCollision(GameObject &other)
{
    for (unsigned i = 0; i < components.size(); i++)
    {
        components[i]->NotifyCollision(other);
    }
}