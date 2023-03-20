#pragma once

#include "Ball.h"

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
	virtual ~AGraphics_Object();

	virtual void Act() = 0;
	virtual void Draw(HDC, RECT &) = 0;
	virtual bool Is_Finished() = 0;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick : public AGraphics_Object
{
protected:
	virtual ~AActive_Brick();
	AActive_Brick(EBrick_Type, int, int);

	EBrick_Type Brick_Type;
	RECT Brick_Rect;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick_Red_Blue : public AActive_Brick
{
public:
	~AActive_Brick_Red_Blue();
	AActive_Brick_Red_Blue(EBrick_Type, int, int);

	virtual void Act();
	virtual void Draw(HDC, RECT&);
	virtual bool Is_Finished();

	static void Setup_Colors();
	static void Draw_In_Level(HDC, RECT &, EBrick_Type);

private:
	int Fade_Step;

	static unsigned char Get_Fading_Channel(unsigned char, unsigned char, int);
	static void Get_Fading_Color(const AColor&, int, AColor &);

	static const int Max_Fade_Step = AsConfig::FPS;

	static AColor Fading_Red_Brick_Colors[Max_Fade_Step];
	static AColor Fading_Blue_Brick_Colors[Max_Fade_Step];
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick_Unbreakable : public AActive_Brick
{
public:
	~AActive_Brick_Unbreakable();
	AActive_Brick_Unbreakable(int, int);

	virtual void Act();
	virtual void Draw(HDC, RECT&);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC, RECT &);

private:
	int Animation_Step;
	HRGN Region;

	static const int Max_Animation_Step = 12;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick_Multihit : public AActive_Brick
{
public:
	~AActive_Brick_Multihit();
	AActive_Brick_Multihit(int, int);

	virtual void Act();
	virtual void Draw(HDC, RECT&);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC, RECT &, EBrick_Type);

private:
	static void Draw_Stage(HDC, RECT &, int, int);

	int Rotation_Step;

	static const int Steps_Per_Turn = 16;
	static const int Max_Rotation_Step = Steps_Per_Turn * 4;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick_Teleport : public AActive_Brick
{
public:
	~AActive_Brick_Teleport();
	AActive_Brick_Teleport(int, int, ABall *);

	virtual void Act();
	virtual void Draw(HDC, RECT &);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC, RECT &, int step = 0);

private:
	int Animation_Step;

	ABall* Ball;

	static const int Max_Animation_Step = 12;
};