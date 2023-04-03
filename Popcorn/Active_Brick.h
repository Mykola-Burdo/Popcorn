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
	EBT_Ad,
	EBT_Invisible
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EDirection_Type
{
	EDT_Left,
	EDT_Up,
	EDT_Right,
	EDT_Down
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick : public AGraphics_Object
{
public:
	void Get_Level_Pos(int &, int &);

protected:
	virtual ~AActive_Brick();
	AActive_Brick(EBrick_Type, int, int);

	virtual void Clear(HDC, RECT&);

	double Get_Brick_X_Pos(bool);
	double Get_Brick_Y_Pos(bool);

	EBrick_Type Brick_Type;
	int Level_X, Level_Y;
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
enum class ETeleport_State
{
	ETS_Starting,
	ETS_Finishing,
	ETS_Done
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick_Teleport : public AActive_Brick
{
public:
	~AActive_Brick_Teleport();
	AActive_Brick_Teleport(int, int, ABall *, AActive_Brick_Teleport *);

	virtual void Act();
	virtual void Draw(HDC, RECT &);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC, RECT &, int step = 0);

	EDirection_Type Release_Direction;

private:
	void Set_Ball(ABall *);

	ETeleport_State Teleport_State;

	int Animation_Step;

	ABall *Ball;
	AActive_Brick_Teleport *Destination_Teleport;

	static const int Max_Animation_Step = 12;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AAdvertisement : public AGraphics_Object
{
public:
	~AAdvertisement();
	AAdvertisement(int, int, int, int);

	virtual void Act();
	virtual void Clear(HDC, RECT &);
	virtual void Draw(HDC, RECT &);
	virtual bool Is_Finished();

	void Show_Under_Brick(int, int);
	bool Has_Brick_At(int, int);

private:
	int Level_X, Level_Y;
	int Width, Height;
	RECT Ad_Rect;
	HRGN Empty_Region;
	int Ball_X, Ball_Y;
	int Ball_Width, Ball_Height;
	int Ball_Y_Offset;
	double Falling_Speed, Acceleration_Step;
	double Deformation_Ratio;

	HRGN *Brick_Regions;

	static const int Ball_Size = 12;
	static const int High_Ball_Threshold = 12 * AsConfig::Global_Scale;
	static const int Low_Ball_Threshold = -3 * AsConfig::Global_Scale;
	static const int Deformation_Height = 2 * AsConfig::Global_Scale;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AActive_Brick_Ad : public AActive_Brick
{
public:
	~AActive_Brick_Ad();
	AActive_Brick_Ad(int, int, AAdvertisement *);

	virtual void Act();
	virtual void Draw(HDC, RECT&);
	virtual bool Is_Finished();

	static void Draw_In_Level(HDC, RECT&);

private:
	AAdvertisement *Advertisement;

	static const int Circle_Size = 7;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
