#include "Text.h"
#include "Camera.h"
#include "Game.h"
#include "Resources.h"

Text::Text(GameObject &associated, std::string fontFile, int fontSize, TextStyle style, std::string text, SDL_Color color, float fade, float lifespan)
    : Component(associated), fontFile(fontFile), fontSize(fontSize), style(style), text(text), color(color), texture(nullptr), cooldown(), showText(true), fade(fade),
      lifespan(lifespan), alpha(0)
{
    MakeTexture();
    if (fade > 0)
    {
        SDL_SetTextureAlphaMod(texture, alpha);
    }
}

Text::~Text()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
}

void Text::Update(float dt)
{
    if (fade > 0 and alpha < 255)
    {
        SDL_SetTextureAlphaMod(texture, alpha);
        alpha += 2;
    }

    if (lifespan > 0)
    {
        cooldown.Update(dt);
        if(cooldown.Get() >= lifespan)
        {
            associated.RequestDelete();
        }
    }
}

void Text::Render()
{
    if (texture != nullptr and showText)
    {
        SDL_Rect src;
        SDL_Rect dst;

        src.x = 0;
        src.y = 0;
        src.w = associated.box.w;
        src.h = associated.box.h;

        dst.x = associated.box.x - Camera::pos.x;
        dst.y = associated.box.y - Camera::pos.y;
        dst.w = src.w;
        dst.h = src.h;

        SDL_RenderCopyEx(Game::GetInstance().GetRenderer(), texture, &src, &dst, 0, nullptr, SDL_FLIP_NONE);
    }
}

void Text::SetText(std::string text)
{
    this->text = text;
    RemakeTexture();
}
void Text::SetFontFile(std::string fontFile)
{
    this->fontFile = fontFile;
    RemakeTexture();
}
void Text::SetColor(SDL_Color color)
{
    this->color = color;
    RemakeTexture();
}
void Text::SetStyle(TextStyle style)
{
    this->style = style;
    RemakeTexture();
}
void Text::SetFontSize(int fontSize)
{
    this->fontSize = fontSize;
    RemakeTexture();
}

void Text::MakeTexture()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
    font = Resources::GetFont(fontFile, fontSize);
    SDL_Surface *surf = TTF_RenderText_Blended(font.get(), text.c_str(), color);
    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surf);
    associated.box.w = surf->w;
    associated.box.h = surf->h;
    associated.box.x -= surf->w / 2;
    SDL_FreeSurface(surf);
}

void Text::RemakeTexture()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
    }
    font = Resources::GetFont(fontFile, fontSize);
    SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(font.get(), text.c_str(), color, 800);
    texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRenderer(), surf);
    associated.box.w = surf->w;
    associated.box.h = surf->h;
    SDL_FreeSurface(surf);
}