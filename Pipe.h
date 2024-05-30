
#ifndef PIPE_H_
#define PIPE_H_

#include "CommonFunction.h"
#include "Object.h"

using namespace std;

class Pipe : Object
{
public:
    Pipe();
    ~Pipe();
    bool LoadImg(string path, SDL_Renderer* screen);
    void SetPs(const int& xp, const int& yp);
    void SetXPs(const int& xp);
    SDL_Rect GetRectPs() const { return this->GetRect(); }
    void ShowImg(SDL_Renderer* screen);
    void DoRun(UINT& x_vtoc);
    bool GetStateBack() const { return need_back_; }
    void SetBack(bool b) { need_back_ = b; }

private:
    bool need_back_;
    int x_ps_;
    int y_ps_;
};



class PipeInit
{
public:
    PipeInit();
    ~PipeInit();
    bool Init(SDL_Renderer* screen, const int& xp);
    void RenderImg(SDL_Renderer* screen);
    void DoFlying();
    bool GetNeedBack() const { return need_back_; }
    void SetNeedBack(bool NeedBack);
    SDL_Rect GetTopRect();
    void SetRectVal(const unsigned int& xp);
    void GetRectSlot();
    bool CheckPass(SDL_Rect rect);
    bool GetPassed() const { return passed_; }
    void SetPassed(const bool& pp) { passed_ = pp; }
    bool CheckCol(SDL_Rect pl_rect);

private:
    Pipe Top_;
    Pipe Bot_;
    unsigned int x_vtoc_;
    bool need_back_;
    SDL_Rect slot_rect_;
    bool passed_;
};


class PipeManager
{
public:
    PipeManager();
    ~PipeManager();
    bool InitPipeList(SDL_Renderer* screen);
    void Render(SDL_Renderer* screen);
    void SetStopFlying(const bool& stop);
    void SetPlayerRect(SDL_Rect pRect) { player_rect_ = pRect; }
    bool GetColState() const { return is_col_; }
    int GetCount() const { return m_cnt; }
    void FreePipe();

private:
    vector<PipeInit*> PipeList;
    unsigned int xp_max_;
    bool stop_flying_;
    SDL_Rect player_rect_;
    int m_cnt; // score
    bool is_col_;
    bool gameover_;
};
#endif


