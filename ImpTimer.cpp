#include "ImpTimer.h"
#include "CommonFunction.h"

ImpTimer::ImpTimer()
{
    start_tick_ = 0;
    paused_ticks_ = 0;
    paused_ = false;
    started_ = false;
}

void ImpTimer::start()
{
    started_ = true;
    paused_ = false;
    start_tick_ = SDL_GetTicks();
}

void ImpTimer::stop()
{
    started_ = false;
    paused_ = false;
}

void ImpTimer::pause()
{
    if (started_ && !paused_)
    {
        paused_ = true;
        paused_ticks_ = SDL_GetTicks() - start_tick_;
    }
}

void ImpTimer::unpause()
{
    if (paused_)
    {
        paused_ = false;
        start_tick_ = SDL_GetTicks() - paused_ticks_;
        paused_ticks_ = 0;
    }
}

int ImpTimer::get_ticks()
{
    if (started_)
    {
        if (paused_)
        {
            return paused_ticks_;
        }
        else
        {
            return SDL_GetTicks() - start_tick_;
        }
    }
    return 0;
}

bool ImpTimer::started()
{
    return started_;
}

bool ImpTimer::paused()
{
    return paused_;
}

