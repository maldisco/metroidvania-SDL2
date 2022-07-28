#ifndef TIMER_H
#define TIMER_H

/**
 * @brief Control time-related mechanics over game objects
 *
 */
class Timer
{
public:
    Timer();
    void Update(float dt);
    void Restart();
    float Get();

private:
    float time;
};
#endif