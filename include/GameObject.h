#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Rect.h"
#include <vector>
#include <memory>

// forward declaration to dodge circular dependency
class Component;

/**
 * @brief represents an entity
 *
 */
class GameObject
{
public:
    Rect box;
    bool started;
    double angleDeg;

    GameObject();

    ~GameObject();

    void Start();

    void Update(float dt);

    void Render();

    bool IsDead();

    // object died/destroyed
    void RequestDelete();

    void AddComponent(Component *cpt);

    void RemoveComponent(Component *cpt);

    void NotifyCollision(GameObject &other);

    Component *GetComponent(std::string type);

private:
    // every component is a part of the entity e.g sprite, sound...
    std::vector<std::unique_ptr<Component>> components;
    bool isDead;
};
#endif