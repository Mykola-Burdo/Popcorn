#include "Active_Brick.h"

//--------------AGraphics_Object--------------------
AGraphics_Object::~AGraphics_Object()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------



//--------------AActive_Brick--------------------
AActive_Brick::~AActive_Brick()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick::AActive_Brick(EBrick_Type brick_type, int level_x, int level_y)
	: Brick_Type(brick_type), Level_X(level_x), Level_Y(level_y), Brick_Rect{}
{
	Brick_Rect.left = (AsConfig::Level_X_Offset + level_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
	Brick_Rect.top = (AsConfig::Level_Y_Offset + level_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
	Brick_Rect.right = Brick_Rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
	Brick_Rect.bottom = Brick_Rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




//--------------AActive_Brick_Red_Blue--------------------
AColor AActive_Brick_Red_Blue::Fading_Red_Brick_Colors[Max_Fade_Step];
AColor AActive_Brick_Red_Blue::Fading_Blue_Brick_Colors[Max_Fade_Step];
//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Red_Blue::~AActive_Brick_Red_Blue()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Red_Blue::AActive_Brick_Red_Blue(EBrick_Type brick_type, int level_x, int level_y)
	: AActive_Brick(brick_type, level_x, level_y), Fade_Step(0)
{
	if(!(brick_type == EBrick_Type::EBT_Red || brick_type == EBrick_Type::EBT_Blue))
		AsConfig::Throw();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Red_Blue::Act()
{
	if (Fade_Step < Max_Fade_Step - 1)
	{
		++Fade_Step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Red_Blue::Draw(HDC hdc, RECT &paint_area)
{
	AColor *color = 0;

	switch (Brick_Type)
	{
	case EBrick_Type::EBT_Red:
		color = &Fading_Red_Brick_Colors[Fade_Step];
		break;

	case EBrick_Type::EBT_Blue:
		color = &Fading_Blue_Brick_Colors[Fade_Step];
		break;
	}

	if (color != 0)
		color->Select(hdc);

	AsConfig::Round_Rect(hdc, Brick_Rect);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AActive_Brick_Red_Blue::Is_Finished()
{
	if (Fade_Step >= Max_Fade_Step - 1)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Red_Blue::Setup_Colors()
{
	for(int i = 0; i < Max_Fade_Step; ++i)
	{
		Get_Fading_Color(AsConfig::Red_Color, i, Fading_Red_Brick_Colors[i]);
		Get_Fading_Color(AsConfig::Blue_Color, i, Fading_Blue_Brick_Colors[i]);
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Red_Blue::Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{// Output of an inactive brick on a level

	const AColor* color = 0;

	switch (brick_type)
	{
	case EBrick_Type::EBT_None:
		color = &AsConfig::BG_Color;
		break;

	case EBrick_Type::EBT_Red:
		color = &AsConfig::Red_Color;
		break;

	case EBrick_Type::EBT_Blue:
		color = &AsConfig::Blue_Color;
		break;

	default:
		AsConfig::Throw();
	}

	if(color != 0)
		color->Select(hdc);

	AsConfig::Round_Rect(hdc, brick_rect);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

unsigned char AActive_Brick_Red_Blue::Get_Fading_Channel(unsigned char color, unsigned char bg_color, int step)
{
	return color - step * (color - bg_color) / (Max_Fade_Step - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Red_Blue::Get_Fading_Color(const AColor &origin_color, int step, AColor &result_color)
{
	unsigned char r, g, b;

	r = Get_Fading_Channel(origin_color.R, AsConfig::BG_Color.R, step);
	g = Get_Fading_Channel(origin_color.G, AsConfig::BG_Color.G, step);
	b = Get_Fading_Channel(origin_color.B, AsConfig::BG_Color.B, step);

	result_color = AColor(r, g, b);
}




//--------------AActive_Brick_Unbreakable--------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Unbreakable::~AActive_Brick_Unbreakable()
{
	DeleteObject(Region);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Unbreakable::AActive_Brick_Unbreakable(int level_x, int level_y)
	: AActive_Brick(EBrick_Type::EBT_Unbreakable, level_x, level_y), Animation_Step(0), Region(0)
{
	Region = CreateRoundRectRgn(Brick_Rect.left, Brick_Rect.top, Brick_Rect.right + 1, Brick_Rect.bottom + 1, 2 * AsConfig::Global_Scale - 1, 2 * AsConfig::Global_Scale - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Unbreakable::Act()
{
	if (Animation_Step <= Max_Animation_Step)
	{
		++Animation_Step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Unbreakable::Draw(HDC hdc, RECT& paint_area)
{
	int offset;
	const int scale = AsConfig::Global_Scale;

	Draw_In_Level(hdc, Brick_Rect);
	
	SelectClipRgn(hdc, Region);

	offset = 2 * Animation_Step * scale - AsConfig::Brick_Width * scale;

	AsConfig::Unbreakable_Blue_Highlight.Select_Pen(hdc);
	MoveToEx(hdc, Brick_Rect.left + 4 * scale + offset, Brick_Rect.bottom + scale, 0);
	LineTo(hdc, Brick_Rect.left + 13 * scale + offset - 1, Brick_Rect.top - 1 * scale);

	AsConfig::Unbreakable_Red_Highlight.Select_Pen(hdc);
	MoveToEx(hdc, Brick_Rect.left + 6 * scale + offset, Brick_Rect.bottom + scale, 0);
	LineTo(hdc, Brick_Rect.left + 15 * scale + offset - 1, Brick_Rect.top - 1 * scale);

	SelectClipRgn(hdc, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AActive_Brick_Unbreakable::Is_Finished()
{
	if (Animation_Step >= Max_Animation_Step)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Unbreakable::Draw_In_Level(HDC hdc, RECT& brick_rect)
{// Output of an inactive brick on a level

	AsConfig::White_Color.Select(hdc);

	AsConfig::Round_Rect(hdc, brick_rect);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




//--------------AActive_Brick_Multihit--------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Multihit::~AActive_Brick_Multihit()
{
	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Multihit::AActive_Brick_Multihit(int level_x, int level_y)
	: AActive_Brick(EBrick_Type::EBT_Multihit_1, level_x, level_y), Rotation_Step(0)
{
	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Multihit::Act()
{
	if (Rotation_Step <= Max_Rotation_Step)
	{
		++Rotation_Step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Multihit::Draw(HDC hdc, RECT& paint_area)
{
	int step;
	const int scale = AsConfig::Global_Scale;
	double x_ratio, rotation_angle;
	RECT zero_rect;
	XFORM xform, old_xform;

	// Cleaning up the background
	AsConfig::BG_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, Brick_Rect);

	// Setting the rotation matrix "100"
	step = Rotation_Step % Steps_Per_Turn;
	rotation_angle = M_PI_4 / 2.0 * (double)step;
	x_ratio = cos(rotation_angle);

	xform.eM11 = (float)x_ratio;
	xform.eM12 = 0.0f;
	xform.eM21 = 0.0f;
	xform.eM22 = 1.0f;
	xform.eDx = (float)Brick_Rect.left + (float)(1.0 - x_ratio) * (float)(AsConfig::Brick_Width * AsConfig::Global_Scale) / 2.0f;
	xform.eDy = (float)Brick_Rect.top;

	GetWorldTransform(hdc, &old_xform);
	SetWorldTransform(hdc, &xform);

	// Draw "100"
	AsConfig::Letter_Color.Select_Pen(hdc);
	MoveToEx(hdc, 0 + 1 * scale + 1, 0 + 3 * scale, 0);
	LineTo(hdc, 0 + 3 * scale + 1, 0 + 1 * scale);
	LineTo(hdc, 0 + 3 * scale + 1, 0 + 6 * scale - 1);

	zero_rect.left = 0 + 5 * scale + 1;
	zero_rect.top = 0 + 1 * scale;
	zero_rect.right = zero_rect.left + 3 * scale + 1;
	zero_rect.bottom = zero_rect.top + 5 * scale;
	AsConfig::Round_Rect(hdc, zero_rect);

	zero_rect.left += 5 * scale;
	zero_rect.right += 5 * scale;
	AsConfig::Round_Rect(hdc, zero_rect);

	SetWorldTransform(hdc, &old_xform);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AActive_Brick_Multihit::Is_Finished()
{
	if (Rotation_Step >= Max_Rotation_Step)
		return true;
	else
		return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Multihit::Draw_In_Level(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{// Output of an inactive brick on a level

	const int scale = AsConfig::Global_Scale;

	// Draw the background
	AsConfig::White_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	AsConfig::Red_Color.Select(hdc);
	Rectangle(hdc, brick_rect.left + scale, brick_rect.top + scale, brick_rect.right - scale - 1, brick_rect.bottom - scale - 1);

	// Draw inner rectangles
	switch (brick_type)
	{
	case EBrick_Type::EBT_Multihit_1:
		Draw_Stage(hdc, brick_rect, 2, 10);
		break;
	case EBrick_Type::EBT_Multihit_2:
		Draw_Stage(hdc, brick_rect, 2, 4);
		Draw_Stage(hdc, brick_rect, 8, 4);
		break;
	case EBrick_Type::EBT_Multihit_3:
		Draw_Stage(hdc, brick_rect, 2, 2);
		Draw_Stage(hdc, brick_rect, 6, 2);
		Draw_Stage(hdc, brick_rect, 10, 2);
		break;
	case EBrick_Type::EBT_Multihit_4:
		Draw_Stage(hdc, brick_rect, 2, 2);
		Draw_Stage(hdc, brick_rect, 5, 2);
		Draw_Stage(hdc, brick_rect, 8, 2);
		Draw_Stage(hdc, brick_rect, 11, 2);
		break;

	default:
		AsConfig::Throw();
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Multihit::Draw_Stage(HDC hdc, RECT &brick_rect, int x, int width)
{// Draw the inner rectangle

	const int scale = AsConfig::Global_Scale;
	RECT stage_rect;

	stage_rect.left = brick_rect.left + x * scale;
	stage_rect.top = brick_rect.top + 2 * scale;
	stage_rect.right = stage_rect.left + width * scale;
	stage_rect.bottom = stage_rect.top + 3 * scale;

	AsConfig::BG_Color.Select(hdc);
	Rectangle(hdc, stage_rect.left + scale, stage_rect.top + scale, stage_rect.right + scale - 1, stage_rect.bottom + scale - 1);

	AsConfig::Blue_Color.Select(hdc);
	Rectangle(hdc, stage_rect.left, stage_rect.top, stage_rect.right - 1, stage_rect.bottom - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------



//--------------AActive_Brick_Teleport--------------------

//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Teleport::~AActive_Brick_Teleport()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Teleport::AActive_Brick_Teleport(int level_x, int level_y, ABall *ball, AActive_Brick_Teleport *destination_teleport)
	: AActive_Brick(EBrick_Type::EBT_Teleport, level_x, level_y), Teleport_State(ETeleport_State::ETS_Starting), Animation_Step(0), Ball(0), Destination_Teleport(destination_teleport)
{
	Set_Ball(ball);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Teleport::Act()
{
	double ball_x, ball_y;

	if (Animation_Step <= Max_Animation_Step)
	{
		++Animation_Step;
		InvalidateRect(AsConfig::Hwnd, &Brick_Rect, FALSE);
	}
	else
	{
		switch (Teleport_State)
		{
		case ETeleport_State::ETS_Starting:
			Animation_Step = 0;
			Teleport_State = ETeleport_State::ETS_Finishing;

			if(Destination_Teleport != 0)
			{
				Destination_Teleport->Set_Ball(Ball);
				Ball = 0;
			}
			break;

		case ETeleport_State::ETS_Finishing:
			Teleport_State = ETeleport_State::ETS_Done;

			if(Ball != 0)
			{
				Ball->Get_Center(ball_x, ball_y);
				Ball->Set_State(EBall_State::EBS_Normal, ball_x, ball_y);
			}

			break;
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Teleport::Draw(HDC hdc, RECT &paint_area)
{
	int step;

	Draw_In_Level(hdc, Brick_Rect, Animation_Step);

	switch (Teleport_State)
	{
	case ETeleport_State::ETS_Starting:
		step = Animation_Step;
		break;

	case ETeleport_State::ETS_Finishing:
		step = Max_Animation_Step - Animation_Step;
		break;

	default:
		return;
	}

	if(Ball != 0)
		Ball->Draw_Teleporting(hdc, step);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AActive_Brick_Teleport::Is_Finished()
{
	/*if (Animation_Step >= Max_Animation_Step)
		return true;
	else*/
		return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Teleport::Draw_In_Level(HDC hdc, RECT &brick_rect, int step)
{// Output of an inactive brick on a level

	const int scale = AsConfig::Global_Scale;
	int top_y = brick_rect.top + step / 2 + 1;
	int low_y = brick_rect.top + 6 * scale - step / 2 + 1;

	// Background
	AsConfig::Red_Color.Select(hdc);
	AsConfig::Round_Rect(hdc, brick_rect);

	// Portal
	AsConfig::Teleport_Portal_Color.Select(hdc);
	Ellipse(hdc, brick_rect.left + 3 * scale + 1, top_y, brick_rect.left + 11 * scale + 1, low_y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AActive_Brick_Teleport::Set_Ball(ABall* ball)
{
	double ball_x, ball_y;

	// Place the ball in the center of the brick
	ball_x = (double)(AsConfig::Level_X_Offset + Level_X * AsConfig::Cell_Width) + (double)AsConfig::Brick_Width / 2.0;
	ball_y = (double)(AsConfig::Level_Y_Offset + Level_Y * AsConfig::Cell_Height) + (double)AsConfig::Brick_Height / 2.0;

	if(ball != 0)
		ball->Set_State(EBall_State::EBS_Teleporting, ball_x, ball_y);

	Ball = ball;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
