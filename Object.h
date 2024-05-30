#ifndef OBJECT_H_
#define OBJECT_H_

#include "CommonFunction.h"

using namespace std;

class Object
{
protected:
	SDL_Texture* texture_;
	SDL_Rect rect_;

public:
	Object();
	~Object();

	bool LoadImageFile(string path, SDL_Renderer* screen);

	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);

	void Free();

	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; };
	SDL_Rect GetRect() const { return rect_; }

	SDL_Texture* GetTexture() { return texture_; }

};

#endif


