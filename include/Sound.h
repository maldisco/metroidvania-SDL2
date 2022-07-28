#ifndef SOUND_H
#define SOUND_H

#include <memory>

#include "Component.h"
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL
#include "SDL_include.h"

/**
 * @brief Component responsible for sound effects.
 *
 */
class Sound : public Component
{
public:
    /**
     * @brief Construct a new Sound object
     *
     * @param associated
     */
    Sound(GameObject &associated);
    Sound(std::string file, GameObject &associated);

    ~Sound();

    void Play(int times = 1);
    void Stop();

    void Open(std::string file);
    bool IsOpen();

    void Update(float dt);
    void Render();
    void NotifyCollision(GameObject &other);

    bool Is(std::string type);

private:
    std::shared_ptr<Mix_Chunk> chunk;
    int channel;
};
#endif