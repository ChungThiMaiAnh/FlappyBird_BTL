#include "Pipe.h"
#include "ImpTimer.h"
#include <SDL_mixer.h>

using namespace std;

int g_ps[] = { 500, 750, 1050 };

Mix_Music* background_music = nullptr;

Pipe::Pipe()
{
    need_back_ = false;
}

Pipe::~Pipe()
{
}

bool Pipe::LoadImg(string path, SDL_Renderer* screen)
{
    return Object::LoadImageFile(path, screen);
}

void Pipe::SetPs(const int& xp, const int& yp)
{
    this->SetRect(xp, yp);
}

void Pipe::SetXPs(const int& xp)
{
    SDL_Rect rect = this->GetRect();
    this->SetRect(xp, rect.y);
}

void Pipe::ShowImg(SDL_Renderer* screen)
{
    this->Render(screen);
}

void Pipe::DoRun(UINT& x_vtoc)
{
    this->rect_.x += x_vtoc;
    if (rect_.x + rect_.w < 0)
    {
        need_back_ = true;
    }
}

PipeInit::PipeInit()
{
    x_vtoc_ = -5;
    need_back_ = false;
    passed_ = false;
}

PipeInit::~PipeInit()
{
}

bool PipeInit::Init(SDL_Renderer* screen, const int& xp)
{
    int num = SDLCommonFunction::MyRandom(1, 10);
    int offset = num * 14;

    bool top = Top_.LoadImg("img//top.png", screen);
    bool bot = Bot_.LoadImg("img//bottom.png", screen);

    if (num % 2 == 0)
    {
        Top_.SetPs(xp, -320 + offset);
        Bot_.SetPs(xp, GROUND_MAP - 220 + offset);
    }
    else
    {
        Top_.SetPs(xp, -320 - offset);
        Bot_.SetPs(xp, GROUND_MAP - 220 - offset);
    }

    return top && bot;
}

void PipeInit::RenderImg(SDL_Renderer* screen)
{
    Top_.ShowImg(screen);
    Bot_.ShowImg(screen);
}

void PipeInit::DoFlying()
{
    Top_.DoRun(x_vtoc_);
    Bot_.DoRun(x_vtoc_);
    if (Top_.GetStateBack() || Bot_.GetStateBack())
    {
        need_back_ = true;
    }
}

SDL_Rect PipeInit::GetTopRect()
{
    return Top_.GetRectPs();
}

void PipeInit::SetRectVal(const UINT& xp)
{
    Top_.SetXPs(xp);
    Bot_.SetXPs(xp);
}

void PipeInit::SetNeedBack(bool NeedBack)
{
    need_back_ = NeedBack;
    Top_.SetBack(NeedBack);
    Bot_.SetBack(NeedBack);
}

void PipeInit::GetRectSlot()
{
    SDL_Rect rect_top = Top_.GetRectPs();

    slot_rect_.x = rect_top.x + rect_top.w;
    slot_rect_.y = rect_top.y + rect_top.h;
    slot_rect_.w = 5;
    slot_rect_.h = 160;
}

bool PipeInit::CheckPass(SDL_Rect rect)
{
    return SDLCommonFunction::CheckCollision(rect, slot_rect_);
}

bool PipeInit::CheckCol(SDL_Rect pl_rect)
{
    bool ret1 = SDLCommonFunction::CheckCollision(pl_rect, Top_.GetRectPs());
    bool ret2 = SDLCommonFunction::CheckCollision(pl_rect, Bot_.GetRectPs());

    return ret1 || ret2;
}

PipeManager::PipeManager()
{
    xp_max_ = 0;
    stop_flying_ = false;
    m_cnt = 0;
    is_col_ = false;
}

PipeManager::~PipeManager()
{
    FreePipe();
    if (background_music != nullptr) {
        Mix_FreeMusic(background_music);
        background_music = nullptr;
    }
    Mix_CloseAudio();
}

void PipeManager::FreePipe()
{
    for (auto pipe : PipeList)
    {
        delete pipe;
    }
    PipeList.clear();
}

bool PipeManager::InitPipeList(SDL_Renderer* screen)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    background_music = Mix_LoadMUS("sound/RunningAway.mp3");

    if (background_music == nullptr) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    if (Mix_PlayMusic(background_music, -1) == -1) {
        printf("Failed to play background music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    PipeInit* Pipe1 = new PipeInit();
    PipeInit* Pipe2 = new PipeInit();
    PipeInit* Pipe3 = new PipeInit();

    if (!Pipe1->Init(screen, g_ps[0]) ||
        !Pipe2->Init(screen, g_ps[1]) ||
        !Pipe3->Init(screen, g_ps[2]))
    {
        return false;
    }

    PipeList.push_back(Pipe1);
    PipeList.push_back(Pipe2);
    PipeList.push_back(Pipe3);

    xp_max_ = 2;
    return true;
}

void PipeManager::SetStopFlying(const bool& stop)
{
    stop_flying_ = stop;
}

void PipeManager::Render(SDL_Renderer* screen)
{
    for (auto& Pipe_ : PipeList)
    {
        Pipe_->GetRectSlot();

        if (!stop_flying_)
        {
            Pipe_->DoFlying();

            if (Pipe_->GetNeedBack())
            {
                PipeInit* endPipe = PipeList.at(xp_max_);
                SDL_Rect end_rect = endPipe->GetTopRect();
                unsigned int xp = end_rect.x + 300;
                Pipe_->SetRectVal(xp);
                Pipe_->SetNeedBack(false);
                Pipe_->SetPassed(false);
                xp_max_ = &Pipe_ - &PipeList[0];
            }

            if (Pipe_->CheckCol(player_rect_))
            {
                is_col_ = true;
                Mix_Chunk* beep_sound = Mix_LoadWAV("sound//punch.wav");
                if (beep_sound != nullptr)
                {
                    Mix_PlayChannel(-1, beep_sound, 0);
                }
                break;
            }

            if (Pipe_->CheckPass(player_rect_) && !Pipe_->GetPassed())
            {
                Mix_Chunk* beep_sound = Mix_LoadWAV("sound//ting.wav");
                if (beep_sound != nullptr)
                {
                    Mix_PlayChannel(-1, beep_sound, 0);
                }
                m_cnt++;
                Pipe_->SetPassed(true);
            }
        }

        Pipe_->RenderImg(screen);
    }
}

