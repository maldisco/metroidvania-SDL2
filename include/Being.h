#ifndef BEING_H
#define BEING_H
#include "Component.h"
#include "TileMap.h"
#include "TileSet.h"

class Being : public Component
{
public:
    /**
     * @brief Construct a new Being object
     *
     * @param associated
     * @param speed
     * @param mass
     * @param hp
     */
    Being(GameObject &associated, Vec2 speed, float mass, int hp, int dir = 1);

    virtual ~Being();

    /**
     * @brief Move in x axis (if possible)
     * 
     * @param motion 
     * @param colliderBox 
     * @param tileMap 
     * @param tileSet 
     */
    virtual void moveX(float motion, Rect colliderBox, TileMap *tileMap, TileSet *tileSet);

    /**
     * @brief Move in Y axis (if possible)
     *
     * @param motion
     * @param colliderBox
     * @param tileMap
     * @param tileSet
     */
    virtual void moveY(float motion, Rect colliderBox, TileMap *tileMap, TileSet *tileSet);

    /**
     * @brief Get the Dir object
     *
     * @return int
     */
    virtual int GetDir();

    /**
     * @brief Get the Speed object
     *
     * @return Vec2
     */
    virtual Vec2 GetSpeed();

    /**
     * @brief Get the Hp object
     * 
     * @return int 
     */
    virtual int GetHp();

    virtual bool Grounded();

protected:
    Vec2 speed;
    float mass;
    int hp, dir;
    bool grounded;
    enum STATE
    {
        IDLE,
        WALKING,
        JUMPING,
        FALLING,
        ATTACKING,
        DASHING,
        HURT,
        DEAD,
        WALLSLIDING
    };
    STATE charState;
};
#endif