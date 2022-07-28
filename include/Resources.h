#ifndef RESOURCES_H
#define RESOURCES_h

#include <unordered_map>
#include <string>
#include <memory>
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_TTF
#include "SDL_include.h"

/**
 * @brief Class that load, alocate and desalocate all resources.
 *
 */
class Resources
{
public:
    /**
     * @brief Load an image
     *
     * @param file
     * @return std::shared_ptr<SDL_Texture*>
     */
    static std::shared_ptr<SDL_Texture> GetImage(std::string file);
    /**
     * @brief Clear images map
     *
     */
    static void ClearImages();

    /**
     * @brief Load a music
     *
     * @param file
     * @return std::shared_ptr<Mix_Music>
     */
    static std::shared_ptr<Mix_Music> GetMusic(std::string file);
    /**
     * @brief Clear musics map
     *
     */
    static void ClearMusics();

    /**
     * @brief Load a sound
     *
     * @param file
     * @return std::shared_ptr<Mix_Chunk>
     */
    static std::shared_ptr<Mix_Chunk> GetSound(std::string file);
    /**
     * @brief Clear sounds map
     *
     */
    static void ClearSounds();

    /**
     * @brief Load a font
     *
     * @param file
     * @param size
     * @return std::shared_ptr<TTF_Font>
     */
    static std::shared_ptr<TTF_Font> GetFont(std::string file, int size);
    /**
     * @brief Clear fonts map
     *
     */
    static void ClearFonts();

private:
    /**
     * @brief Maps will be used for memory usage improvement
     *
     */
    static std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> imageTable;
    static std::unordered_map<std::string, std::shared_ptr<Mix_Music>> musicTable;
    static std::unordered_map<std::string, std::shared_ptr<Mix_Chunk>> soundTable;
    static std::unordered_map<std::string, std::shared_ptr<TTF_Font>> fontTable;
};
#endif