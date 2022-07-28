#ifndef COMPONENT_H
#define COMPONENT_H

#include "string"
#include "bits/stdc++.h"
#include "GameObject.h"
#include "Rect.h"

/**
 * @brief interface for parts of gameobjects
 *
 */
class Component
{
public:
    Component(GameObject &associated);

    virtual ~Component();

    /**
     * @brief update state (if it exists) of the component
     *
     * @param dt
     */
    virtual void Update(float dt) = 0;

    /**
     * @brief render (if it can) component on game window
     */
    virtual void Render() = 0;

    /**
     * @brief check type of the component
     *
     * @param type
     * @return true
     * @return false
     */
    virtual bool Is(std::string type) = 0;

    /**
     * @brief preparatory actions
     *
     */
    virtual void Start();

    virtual void NotifyCollision(GameObject &other);

    virtual Rect GetBox();

protected:
    GameObject &associated;
};
#endif