#include "Config.h"

//--------------AsConfig--------------------
bool AsConfig::Level_Has_Floor = true;
int AsConfig::Current_Timer_Tick = 0;

const AColor AsConfig::BG_Color(0, 0, 0);
const AColor AsConfig::Red_Color(255, 85,85);
const AColor AsConfig::Blue_Color(85, 255, 255);
const AColor AsConfig::White_Color(255, 255, 255);
const AColor AsConfig::Letter_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Laser_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Gate_Color(AsConfig::White_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Blue_Highlight(AsConfig::Blue_Color, AsConfig::Global_Scale);
const AColor AsConfig::Unbreakable_Red_Highlight(AsConfig::Red_Color, 3 * AsConfig::Global_Scale);
const AColor AsConfig::Teleport_Portal_Color(AsConfig::Blue_Color, AsConfig::BG_Color, AsConfig::Global_Scale);
const AColor AsConfig::Advertisement_Blue_Table(0, 159, 159, AsConfig::Global_Scale);
const AColor AsConfig::Advertisement_Red_Table(AsConfig::Red_Color, 2 * AsConfig::Global_Scale);

HWND AsConfig::Hwnd;

const double AsConfig::D_Global_Scale = (double)Global_Scale;
const double AsConfig::Moving_Step_Size = 1.0 / AsConfig::Global_Scale;
const double AsConfig::Start_Ball_Y_Pos = 184.0;
const double AsConfig::Ball_Acceleration = 1.001;
const double AsConfig::Normal_Ball_Speed = 3.0;
const double AsConfig::Min_Ball_Angle = M_PI / 8.0;
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsConfig::Rand(int range)
{// Calculates a pseudo-random number in a range [0, .., range - 1]

   return rand() * range / RAND_MAX;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsConfig::Round_Rect(HDC hdc, RECT &rect, int corner_radius)
{
   int radius = corner_radius * AsConfig::Global_Scale;

   RoundRect(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1, radius, radius);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsConfig::Rect(HDC hdc, RECT &rect, const AColor &color)
{
   color.Select(hdc);
   Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsConfig::Rect(HDC hdc, int x, int y, int width, int height, const AColor &color)
{
   color.Select(hdc);
   Rectangle(hdc, x * Global_Scale, y * Global_Scale, (x + width) * Global_Scale - 1, (y + height) * Global_Scale - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsConfig::Invalidate_Rect(RECT &rect)
{
   InvalidateRect(AsConfig::Hwnd, &rect, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsConfig::Throw()
{
   throw 13;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
