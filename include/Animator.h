#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Component.h"
#include "Sprite.h"

class AnimationConnection
{
public:
    AnimationConnection(std::string to, std::vector<std::string> conditions) : to(to), conditions(conditions) {}

    std::string to;
    std::vector<std::string> conditions;
};

class Animator : public Component
{
public:
    Animator(GameObject &associated, Sprite *sprite, std::string startingState = "idle");

    void Update(float dt);

    void AddSprite(std::string name, std::string path, float frameTime, float frameCount, int restart = 0);
    void AddCondition(std::string condition);
    void AddConnection(std::string from, std::string to, std::vector<std::string> conditions);
    void ChangeSprite(std::string name);

    void SetCondition(std::string condition, bool value);
    void ResetConditions();

    std::vector<bool> CheckConditions(std::vector<std::string> conditions);
private:
    std::string state;

    struct SpriteConfig
    {
        std::string path;
        float frameTime;
        float frameCount;
        int restart;
    };
    std::map<std::string, SpriteConfig> spriteMap;
    std::map<std::string, bool> conditionMap;
    std::map<std::string, std::vector<AnimationConnection>> connectionMap;

    Sprite *sprite;
};

#endif