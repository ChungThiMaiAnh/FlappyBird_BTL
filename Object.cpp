#include "Object.h"
#include <bits/stdc++.h>

using namespace std;

Object::Object()
    : texture_(nullptr), rect_{0, 0, 0, 0}
{
}

Object::~Object()
{
    Free();
}

bool Object::LoadImageFile(string path, SDL_Renderer* screen)
{
    Free();

    SDL_Texture* newTexture = nullptr;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface != nullptr)
    {
        Uint32 key = SDL_MapRGB(loadedSurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);

        newTexture = SDL_CreateTextureFromSurface(screen, loadedSurface);
        if (newTexture != nullptr)
        {
            rect_.w = loadedSurface->w;
            rect_.h = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    texture_ = newTexture;

    if (texture_ == nullptr)
    {
        cerr << "Failed to load texture from " << path << ". SDL_Error: " << SDL_GetError() << endl;
    }
    return texture_ != nullptr;
}

void Object::Free()
{
    if (texture_ != nullptr)
    {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
        rect_.w = 0;
        rect_.h = 0;
    }
}

void Object::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
    SDL_Rect renderQuad = { rect_.x, rect_.y, rect_.w, rect_.h };
    if (clip != nullptr)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(des, texture_, clip, &renderQuad);
}

