#ifndef SPRITE_H
#define SPRITE_H

#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

#include "Component.h"
#include "Timer.h"

#include <string>

/**
 * @brief Component responsible for in-game texture (image).
 * 
 */
class Sprite : public Component{
    private:
        /**
         * @brief Object that points to a texture 
         */
        std::shared_ptr<SDL_Texture> texture;
        /**
         * @brief Rectangle specifying dimensions of the texture 
         */
        SDL_Rect clipRect;
        Vec2 scale;
        int width;
        int height;
        int frameCount;
        int currentFrame;
        float timeElapsed;
        float frameTime;
        float secondsToSelfDestruct;
        Timer selfDestructCount;
        int dir;
        int restart;
    
    public:
        /**
         * @brief Construct a new Sprite object
         * 
         * @param associated 
         * @param frameCount 
         * @param frameTime 
         * @param secondsToSelfDestruct 
         * @param restart 
         */
        Sprite(GameObject& associated, int frameCount, float frameTime, float secondsToSelfDestruct = 0, int restart = 0);
        
        /**
         * @brief Construct a new Sprite object
         * 
         * @param file 
         * @param associated 
         * @param frameCount 
         * @param frameTime 
         * @param secondsToSelfDestruct 
         * @param restart 
         */
        Sprite(std::string file, GameObject& associated, int frameCount = 1, float frameTime = 1, float secondsToSelfDestruct = 0, int restart = 0);
        ~Sprite();

        /**
         * @brief Load file into texture
         * 
         * @param file 
         */
        void Open(std::string file);

        /**
         * @brief Change texture
         * 
         * @param file 
         * @param frameTime 
         * @param frameCount 
         * @param restart 
         */
        void Change(std::string file, float frameTime, int frameCount, int restart = 0);

        /**
         * @brief Set the Clip object attributes
         * 
         * @param x 
         * @param y 
         * @param w 
         * @param h 
         */
        void SetClip(int x, int y, int w, int h);

        /**
         * @brief Set the Frame object
         * 
         * @param frame 
         */
        void SetFrame(int frame);

        /**
         * @brief Set the Frame Count object
         * 
         * @param frameCount 
         */
        void SetFrameCount(int frameCount);

        /**
         * @brief Set the Frame Time object
         * 
         * @param frameTime 
         */
        void SetFrameTime(float frameTime);
        
        /**
         * @brief Set the Restart object
         * 
         * @param restart 
         */
        void SetRestart(int restart);

        /**
         * @brief Render texture on game window at given coordinates
         * 
         * @param x 
         * @param y 
         */
        void Render(int x, int y);

        /**
         * @brief Render texture on game window 
         */
        void Render();

        void Update(float dt);
        bool Is(std::string type);
        void NotifyCollision(GameObject& other);

        /**
         * @brief Set sprite scale (size)
         * 
         * @param scaleX 
         * @param scaleY 
         */
        void SetScaleX(float scaleX, float scaleY);
        Vec2 GetScale();

        /**
         * @brief Set the Dir object
         * 
         * @param dir 
         */
        void SetDir(int dir);

        /**
         * @brief Get the Width object
         * 
         * @return int 
         */
        int GetWidth();

        /**
         * @brief Get the Height object
         * 
         * @return int 
         */
        int GetHeight();
        
        /**
         * @brief Tell if texture is already loaded
         * 
         * @return true 
         * @return false 
         */
        bool IsOpen();
};
#endif