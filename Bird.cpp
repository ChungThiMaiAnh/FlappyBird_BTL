#include "Bird.h"

using namespace std;

Bird::Bird()
    : falling_(false), y_vtoc_(5)
{
}

Bird::~Bird()
{
    Free();
}

bool Bird::LoadImg(string path, SDL_Renderer* screen)
{
    return Object::LoadImageFile(path, screen);
}

void Bird::Show(SDL_Renderer* des)
{
    Object::Render(des);
}

void Bird::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
    if (events.type == SDL_KEYDOWN)
    {
        if (events.key.keysym.sym == SDLK_SPACE)
        {
            if (!falling_)
            {
                y_vtoc_ = -10;
            }
        }
    }
    else if (events.type == SDL_KEYUP)
    {
        if (events.key.keysym.sym == SDLK_SPACE)
        {
            y_vtoc_ = 4;
        }
    }
}

void Bird::DoFalling(SDL_Renderer* des)
{
    rect_.y += y_vtoc_;
    if ((rect_.y + rect_.h) >= GROUND_MAP)
    {
        falling_ = true;
        DoGround(des);
    }
}

void Bird::DoGround(SDL_Renderer* screen)
{
    y_vtoc_ = -15;
    rect_.y = GROUND_MAP - rect_.h;
    die_ = true;
}

void Bird::reset()
{
    die_ = false;
    falling_ = false;
    y_vtoc_ = 5;
}

