#include "CommonFunction.h"

using namespace std;

int SDLCommonFunction::MyRandom(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

bool SDLCommonFunction::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
    int left1 = object1.x;
    int right1 = object1.x + object1.w;
    int top1 = object1.y;
    int bot1 = object1.y + object1.h;

    int left2 = object2.x;
    int right2 = object2.x + object2.w;
    int top2 = object2.y;
    int bot2 = object2.y + object2.h;

    if (left1 > left2 && left1 < right2)
    {
        if (top1 > top2 && top1 < bot2)
        {
            return true;
        }
    }
    if (left1 > left2 && left1 < right2)
    {
        if (bot1 > top2 && bot1 < bot2)
        {
            return true;
        }
    }
    if (right1 > left2 && right1 < right2)
    {
        if (top1 > top2 && top1 < bot2)
        {
            return true;
        }
    }
    if (right1 > left2 && right1 < right2)
    {
        if (bot1 > top2 && bot1 < bot2)
        {
            return true;
        }
    }

    if (left2 > left1 && left2 < right1)
    {
        if (top2 > top1 && top2 < bot1)
        {
            return true;
        }
    }
    if (left2 > left1 && left2 < right1)
    {
        if (bot2 > top1 && bot2 < bot1)
        {
            return true;
        }
    }
    if (right2 > left1 && right2 < right1)
    {
        if (top2 > top1 && top2 < bot1)
        {
            return true;
        }
    }
    if (right2 > left1 && right2 < right1)
    {
        if (bot2 > top1 && bot2 < bot1)
        {
            return true;
        }
    }

    if (top1 == top2 && right1 == right2 && bot1 == bot2)
    {
        return true;
    }

    return false;
}

int SDLCommonFunction::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
    const string& menu1, const string& menu2,
    const string& img_name, int const score)
{
    char* ch1 = (char*)menu1.c_str();
    char* ch2 = (char*)menu2.c_str();
    char* img_file = (char*)img_name.c_str();

    const int kMenuNum = 2;
    char* labels[kMenuNum] = { new char[menu1.length() + 1], new char[menu2.length() + 1] };

    strcpy_s(labels[0], menu1.length() + 1, ch1);
    strcpy_s(labels[1], menu2.length() + 1, ch2);

    SDL_Texture* menu[kMenuNum];
    bool selected[kMenuNum] = { 0, 0 };

    Text text_object[kMenuNum];
    for (int i = 0; i < kMenuNum; ++i)
    {
        text_object[i].SetText(labels[i]);
        text_object[i].setColor(0, 0, 0);
        text_object[i].loadFromRenderedText(font, g_screen);
    }

    SDL_Rect pos[kMenuNum];
    Object gBackground;
    SDL_Event event;
    int time = 0;
    int x = 0, y = 0;

    while (true)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                text_object[0].Free();
                text_object[1].Free();
                gBackground.Free();
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < kMenuNum; ++i)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(0, 255, 0);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                    else
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(0, 0, 0);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < kMenuNum; ++i)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        text_object[0].Free();
                        text_object[1].Free();
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    text_object[0].Free();
                    text_object[1].Free();
                    gBackground.Free();
                    return 1;
                }
                break;
            }
        }

        gBackground.Render(g_screen, NULL);

        if (score != -1)
        {
            pos[0].x = SCREEN_WIDTH * 0.3 - 150;
            pos[0].y = SCREEN_HEIGHT * 0.8 - 40;
            pos[1].x = SCREEN_WIDTH * 0.3 - 150;
            pos[1].y = SCREEN_HEIGHT * 0.8 + 40;

            gBackground.SetRect(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3);
            gBackground.LoadImageFile(img_file, g_screen);

            Text text_count_, text_score;
            text_count_.setColor(0, 0, 0);
            text_score.setColor(0, 0, 0);
            string count_str = to_string(score);
            text_score.SetText("YOUR SCORE: ");
            text_count_.SetText(count_str);
            text_score.loadFromRenderedText(font, g_screen);
            text_count_.loadFromRenderedText(font, g_screen);
            text_score.RenderText(g_screen, SCREEN_WIDTH * 0.3 - 155, SCREEN_HEIGHT * 0.8 - 120);
            text_count_.RenderText(g_screen, SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT * 0.8 - 120);
        }
        else
        {
            pos[0].x = 340;
            pos[0].y = 515;
            pos[1].x = 360;
            pos[1].y = 565;
            gBackground.LoadImageFile(img_file, g_screen);
        }

        for (int i = 0; i < kMenuNum; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].getWid();
            pos[i].h = text_object[i].getHei();
        }

        SDL_RenderPresent(g_screen);
        if (1000/30 > (SDL_GetTicks() - time))
            SDL_Delay(1000/30 - (SDL_GetTicks() - time));
    }

    return 0;
}

