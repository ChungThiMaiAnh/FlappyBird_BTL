#ifndef TEXT_H_
#define TEXT_H_
#include "CommonFunction.h"

using namespace std;

class Text
{
public:
    Text();
    ~Text();

    bool loadFromRenderedText(TTF_Font* gFont, SDL_Renderer* screen);
    void Free();

    void setColor(Uint8 red, Uint8 green, Uint8 blue);
    void setColor(int type);


    void RenderText(SDL_Renderer* screen, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    int getWid() const {return wid_;}
    int getHei() const {return hei_;}

    void SetText(const string& text) {str_vtoc_ = text;}
    string GetText() const {return str_vtoc_;}

private:
    string str_vtoc_;
    SDL_Color text_color_;
    SDL_Texture* texture_;
    int wid_;
    int hei_;

};

#endif

