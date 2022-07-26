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
    virtual void Update(float dt);

    /**
     * @brief render (if it can) component on game window
     */
    virtual void Render();

    /**
     * @brief preparatory actions
     *
     */
    virtual void Start();

    /**
     * @brief Get the Component object
     * 
     * @tparam T 
     * @return std::common_type<T>::type* 
     */
    template <class T>
    typename std::common_type<T>::type *GetComponent()
    {
        {
            return associated.GetComponent<T>();
        }
    }

    /**
     * @brief Get the associated Box object
     * 
     * @return Rect 
     */
    Rect GetBox()
    {
        return associated.box;
    }

    virtual void NotifyCollision(GameObject &other);

    GameObject &associated;
    bool enabled;
};
#endif