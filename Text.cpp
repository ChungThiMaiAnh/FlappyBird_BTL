#include "Text.h"

Text::Text()
    : wid_(0), hei_(0), texture_(nullptr)
{
    text_color_ = {255, 255, 255}; //white
}

Text::~Text()
{
    Free();
}

void Text::Free()
{
    if (texture_ != nullptr)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
        wid_ = 0;
        hei_ = 0;
    }
}

bool Text::loadFromRenderedText(TTF_Font* gFont, SDL_Renderer* screen)
{
    Free();

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, str_vtoc_.c_str(), text_color_);
    if (textSurface != nullptr)
    {
        texture_ = SDL_CreateTextureFromSurface(screen, textSurface);
        if (texture_ != nullptr)
        {
            wid_ = textSurface->w;
            hei_ = textSurface->h;
        }
        SDL_FreeSurface(textSurface);
    }

    return texture_ != nullptr;
}

void Text::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    text_color_ = {red, green, blue};
}

void Text::RenderText(SDL_Renderer* screen, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, wid_, hei_};

    if (clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(screen, texture_, clip, &renderQuad, angle, center, flip);
}

