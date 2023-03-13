#pragma once

#include "Config.h"

enum class EBrick_Type
{
	EBT_None,

	EBT_Red,
	EBT_Blue,
	EBT_Unbreakable,
	EBT_Multihit_1,
	EBT_Multihit_2,
	EBT_Multihit_3,
	EBT_Multihit_4,
	EBT_Parachute,
	EBT_Teleport,
	EBT_Ad
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AGraphics_Object
{
public:
	virtual void Act() = 0;
	virtual void Draw(HDC, RECT &) = 0;
	virtual bool Is_Finished() = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick : public AGraphics_Object
{
public:
	AActive_Brick(EBrick_Type, int, int);

	virtual void Act();
	virtual void Draw(HDC, RECT&);
	virtual bool Is_Finished();

	static void Setup_Colors();

private:
	static unsigned char Get_Fading_Channel(unsigned char, unsigned char, int);
	static void Get_Fading_Color(const AColor &, int, HPEN &, HBRUSH &);

	EBrick_Type Brick_Type;
	int Fade_Step;
	RECT Brick_Rect;

	static const int Max_Fade_Step = AsConfig::FPS;

	static HPEN Fading_Red_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Red_Brick_Brushes[Max_Fade_Step];
	static HPEN Fading_Blue_Brick_Pens[Max_Fade_Step];
	static HBRUSH Fading_Blue_Brick_Brushes[Max_Fade_Step];
};