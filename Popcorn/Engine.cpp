#include "Engine.h"


enum class EBrick_Type
{
   EBT_None = 0,
   EBT_Red,
   EBT_Blue
};

HPEN Brick_Red_Pen, Brick_Blue_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush;

const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;

char Level_01[14][12] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Init()
{// Game setup at startup
   Brick_Red_Pen = CreatePen(PS_SOLID, 0, RGB(255, 85, 85));
   Brick_Red_Brush = CreateSolidBrush(RGB(255, 85, 85));

   Brick_Blue_Pen = CreatePen(PS_SOLID, 0, RGB(85, 255, 255));
   Brick_Blue_Brush = CreateSolidBrush(RGB(85, 255, 255));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Draw_Brick(HDC hdc, int x, int y, EBrick_Type brick_type)
{// The conclusion of the "brick"

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBrick_Type::EBT_None:
      return;

   case EBrick_Type::EBT_Red:
      pen = Brick_Red_Pen;
      brush = Brick_Red_Brush;
      break;

   case EBrick_Type::EBT_Blue:
      pen = Brick_Blue_Pen;
      brush = Brick_Blue_Brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);
   RoundRect(hdc, x * Global_Scale, y * Global_Scale, (x + Brick_Width) * Global_Scale, (y + Brick_Height) * Global_Scale, 2 * Global_Scale, 2 * Global_Scale);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Draw_Level(HDC hdc)
{// Output all bricks of the level
   int i, j;

   for (i = 0; i < 14; i++)
      for (j = 0; j < 12; j++)
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Draw_Frame(HDC hdc)
{// Drawing the game screen

   Draw_Level(hdc);

}
//-----------------------------------------------------------------------------------------------------------------------------------------------


