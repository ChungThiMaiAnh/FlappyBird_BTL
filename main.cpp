#include <bits/stdc++.h>
#include "CommonFunction.h"
#include "Object.h"
#include "Bird.h"
#include "Pipe.h"
#include "Text.h"
#include "ImpTimer.h"

using namespace std;

Object g_background;
Object g_ground;

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;

bool Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Flappy Bird",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (!g_window) return false;

    g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
    if (!g_screen) return false;

    SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) return false;

    if (TTF_Init() == -1) return false;
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) return false;

    g_font_text = TTF_OpenFont(".\\font\\calibrib.ttf", 36);
    if (!g_font_text) return false;

    g_font_MENU = TTF_OpenFont(".\\font\\calibrib.ttf", 48);
    if (!g_font_MENU) return false;

    return true;
}

bool LoadBackground()
{
    return g_background.LoadImageFile(".\\img\\background.png", g_screen);
}

void Close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    SDL_DestroyWindow(g_window);
    g_screen = nullptr;
    g_window = nullptr;
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (!Init()) return -1;

    ImpTimer fps;
    bool quit = false;

    int ret_menu = SDLCommonFunction::ShowMenu(g_screen, g_font_MENU, "START", "Exit", ".\\img\\MainMenu.png", -1);
    if (ret_menu == 1) quit = true;

    if (!LoadBackground()) return -1;

    if (!g_ground.LoadImageFile(".\\img\\base.png", g_screen)) return -1;
    g_ground.SetRect(0, GROUND_MAP);

    int cnt_again = 0;
    int max = 0;

    while (!quit)
    {
    again_game:
        Text text_count_;
        text_count_.setColor(255, 255, 255);

        Bird player;
        if (!player.LoadImg(".\\img\\bird.png", g_screen)) return -1;
        player.SetRect(100, 100);

        PipeManager manage_pipe;
        if (!manage_pipe.InitPipeList(g_screen)) return -1;

        player.SetFalling(false);
        player.reset();

        bool game_over = false;

        while (!quit && !game_over)
        {
            fps.start();
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT) quit = true;
                player.HandleInputAction(g_event, g_screen);
            }

            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            SDL_RenderClear(g_screen);

            g_background.Render(g_screen, nullptr);
            manage_pipe.SetPlayerRect(player.GetRect());

            if (player.GetFalling()) manage_pipe.SetStopFlying(true);

            manage_pipe.Render(g_screen);

            if (manage_pipe.GetColState()) player.SetFalling(true);

            player.DoFalling(g_screen);
            player.Show(g_screen);

            Text text_count_again, again;
            again.setColor(255, 255, 255);
            text_count_again.SetText(std::to_string(cnt_again));
            again.SetText("AGAIN: ");
            text_count_again.loadFromRenderedText(g_font_text, g_screen);
            text_count_again.RenderText(g_screen, 150, 10);
            again.loadFromRenderedText(g_font_text, g_screen);
            again.RenderText(g_screen, 20, 10);

            int score = manage_pipe.GetCount();
            text_count_.SetText(std::to_string(score));
            text_count_.loadFromRenderedText(g_font_text, g_screen);
            text_count_.RenderText(g_screen, SCREEN_WIDTH / 2, 10);

            if (max < score) max = score;

            Text HighScore, text_high_score;
            HighScore.setColor(255, 255, 255);
            text_high_score.setColor(255, 255, 255);
            HighScore.SetText("HighScore:");
            text_high_score.SetText(std::to_string(max));
            HighScore.loadFromRenderedText(g_font_text, g_screen);
            text_high_score.loadFromRenderedText(g_font_text, g_screen);
            HighScore.RenderText(g_screen, 570, 10);
            text_high_score.RenderText(g_screen, 750, 10);

            g_ground.Render(g_screen);
            SDL_RenderPresent(g_screen);

            game_over = player.GetIsDie();
            if (game_over)
            {
                SDL_Delay(100);
                int ret_menu = SDLCommonFunction::ShowMenu(g_screen, g_font_MENU, "Play again", "Exit", ".\\img\\gameover.png", score);
                if (ret_menu == 1) quit = true;
                else
                {
                    cnt_again++;
                    goto again_game;
                }
            }

            int frame_delay = 1000 / FRAMES_PER_SECOND;
            int frame_time = fps.get_ticks();
            if (frame_delay > frame_time) SDL_Delay(frame_delay - frame_time);
        }
    }

    Close();
    return 0;
}
