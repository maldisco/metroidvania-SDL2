#include "Animator.h"
#include "Helpers.h"

Animator::Animator(GameObject &associated, Sprite *sprite, std::string startingState) : Component(associated), sprite(sprite), state(startingState) {}

void Animator::Update(float dt)
{
    for(auto connection : connectionMap[state])
    {
        if(Helpers::All(CheckConditions(connection.conditions)))
        {
            state = connection.to;
            ChangeSprite(connection.to);
        }
    }
    ResetConditions();
}

void Animator::SetCondition(std::string name, bool value)
{
    conditionMap[name] = value;
}

void Animator::ResetConditions()
{
    for(auto& condition : conditionMap)
    {
        condition.second = false;
    }
}

void Animator::AddSprite(std::string name, std::string path, float frameTime, float frameCount, int restart)
{
    SpriteConfig newSprite;
    newSprite.path = path;
    newSprite.frameTime = frameTime;
    newSprite.frameCount = frameCount;
    newSprite.restart = restart;

    spriteMap.emplace(name, newSprite);
}

void Animator::AddCondition(std::string condition)
{
    conditionMap[condition] = false;
}

void Animator::AddConnection(std::string from, std::string to, std::vector<std::string> conditions)
{
    connectionMap[from].push_back(AnimationConnection(to, conditions));
}

void Animator::ChangeSprite(std::string name)
{
    SpriteConfig newSprite = spriteMap[name];
    sprite->Change(newSprite.path, newSprite.frameTime, newSprite.frameCount, newSprite.restart);
}

std::vector<bool> Animator::CheckConditions(std::vector<std::string> conditions)
{
    std::vector<bool> check;
    for(auto condition : conditions)
    {
        check.push_back(conditionMap[condition]);
    }

    return check;
}