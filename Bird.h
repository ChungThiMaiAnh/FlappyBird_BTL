#ifndef BIRD_H_
#define BIRD_H_

#include "CommonFunction.h"

class Bird : public Object
{
public:

	Bird();
	~Bird();

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
	void DoFalling(SDL_Renderer* des);
	bool GetFalling() const
        {
	    return falling_;
	    }
	void SetFalling(bool falling)
        {
	    falling_ = falling;
	    }
	void DoGround(SDL_Renderer* screen);
	bool GetIsDie() const
        {
	    return die_;
	    }
	void reset() ;

	void Render(SDL_Renderer* ren);
private:
	bool falling_;
	float y_vtoc_;
	bool die_;

};

#endif

