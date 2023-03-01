#pragma once

#include "Config.h"

class AActive_Brick
{
public:
	AActive_Brick();
	void Act(HWND);
	void Draw(HDC, RECT &);

	int Fade_Step;
	RECT Brick_Rect;

	static const int Max_Fade_Step = 80;
};