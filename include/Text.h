#ifndef TEXT_H
#define TEXT_H

#define PEABERRY_FONT "assets/font/PeaberryBase.ttf"

#include "Component.h"
#include "Timer.h"

#include <memory>

#define INCLUDE_SDL_TTF
#define INCLUDE_SDL_IMAGE
#include "SDL_include.h"

class Text : public Component
{
public:
    enum TextStyle
    {
        SOLID,
        SHADED,
        BLENDED
    };

    /**
     * @brief Construct a new Text object
     * 
     * @param associated 
     * @param fontFile 
     * @param fontSize 
     * @param style 
     * @param text 
     * @param color 
     * @param fade 
     * @param lifespan 
     */
    Text(GameObject &associated, std::string fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, float fade = 0, float lifespan = 0);

    ~Text();

    void Update(float dt);
    void Render();

    void SetText(std::string text);
    void SetColor(SDL_Color color);
    void SetStyle(TextStyle style);
    void SetFontFile(std::string fontFile);
    void SetFontSize(int fontSize);

private:
    std::shared_ptr<TTF_Font> font;
    SDL_Texture *texture;
    std::string text;
    TextStyle style;
    std::string fontFile;
    int fontSize;
    SDL_Color color;
    Timer cooldown;
    bool showText;
    float fade, alpha, lifespan;

    void MakeTexture();
    void RemakeTexture();
};
#endif