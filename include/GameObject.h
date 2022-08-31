#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Enums.h"
#include "Rect.h"
#include <vector>
#include <memory>
#include <type_traits>

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
    int direction;
    Enums::Layer layer;

    GameObject(float x = 0, float y = 0, Enums::Layer layer = Enums::Default);

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

    /**
     * @brief Get the Component object
     * 
     * @tparam T 
     * @return std::common_type<T>::type* 
     */
    template <class T>
    typename std::common_type<T>::type *GetComponent()
    {
        for (unsigned i = 0; i < components.size(); i++)
        {
            if (dynamic_cast<T *>(components[i].get()) != nullptr)
            {
                auto component = dynamic_cast<T *>(components[i].get());
                return component;
            }
        }

        return nullptr;
    }

private:
    // every component is a part of the entity e.g sprite, sound...
    std::vector<std::unique_ptr<Component>> components;
    bool isDead;
};
#endif