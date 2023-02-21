#include "Engine.h"

#define _USE_MATH_DEFINES
#include<cmath>

enum class ELetter_Type
{
   ELT_None = 0,
   ELT_O
};

enum class EBrick_Type
{
   EBT_None = 0,
   EBT_Red,
   EBT_Blue
};

HPEN Highlight_Pen, Letter_Pen, Brick_Red_Pen, Brick_Blue_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
HBRUSH Brick_Red_Brush, Brick_Blue_Brush, Platform_Circle_Brush, Platform_Inner_Brush;

const int Brick_Width = 15;
const int Brick_Height = 7;
const int Cell_Width = 16;
const int Cell_Height = 8;
const int Level_X_Offset = 8;
const int Level_Y_Offset = 6;
const int Circle_Size = 7;

int Inner_Width = 21;

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

void Create_Pen_Brush(unsigned char r, unsigned char g, unsigned char b, HPEN &pen, HBRUSH &brush)
{
   pen = CreatePen(PS_SOLID, 0, RGB(r, g, b));
   brush = CreateSolidBrush(RGB(r, g, b));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Init()
{// Game setup at startup

   Highlight_Pen = CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
   Letter_Pen = CreatePen(PS_SOLID, Global_Scale, RGB(255, 255, 255));

   Create_Pen_Brush(255, 85, 85, Brick_Red_Pen, Brick_Red_Brush);

   Create_Pen_Brush(85, 255, 255, Brick_Blue_Pen, Brick_Blue_Brush);

   Create_Pen_Brush(151, 0, 0, Platform_Circle_Pen, Platform_Circle_Brush);

   Create_Pen_Brush(0, 128, 192, Platform_Inner_Pen, Platform_Inner_Brush);
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

void Set_Brick_Letter_Colors(bool is_switch_color, HPEN &front_pen, HBRUSH &front_brush, HPEN &back_pen, HBRUSH &back_brush)
{
   if (is_switch_color)
   {
      front_pen = Brick_Red_Pen;
      front_brush = Brick_Red_Brush;
      
      back_pen = Brick_Blue_Pen;
      back_brush = Brick_Blue_Brush;
   }
   else
   {
      front_pen = Brick_Blue_Pen;
      front_brush = Brick_Blue_Brush;

      back_pen = Brick_Red_Pen;
      back_brush = Brick_Red_Brush; 
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Draw_Brick_Letter(HDC hdc, int x, int y, int rotation_step, EBrick_Type brick_type, ELetter_Type letter_type)
{// Falling letter output

   bool switch_color;
   double offset;
   double rotation_angle; // Pitch to angle conversion
   int brick_half_height = Brick_Height * Global_Scale / 2;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(brick_type == EBrick_Type::EBT_Blue || brick_type == EBrick_Type::EBT_Red))
      return;// Falling letters can only be from this type of bricks

   // Correction of rotation pitch and angle of rotation
   rotation_step %= 16;

   if (rotation_step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)rotation_step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8L - (long long)rotation_step);

   if (rotation_step > 4 && rotation_step <= 12)
   {
      if (brick_type == EBrick_Type::EBT_Blue)
         switch_color = true;
      else
         switch_color = false;
   }
   else
   {
      if (brick_type == EBrick_Type::EBT_Red)
         switch_color = true;
      else
         switch_color = false;
   }

   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (rotation_step == 4 || rotation_step == 12)
   {
      // Background output
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      RoundRect(hdc, x, y + brick_half_height - Global_Scale, x + Brick_Width * Global_Scale, y + brick_half_height, 3 * Global_Scale, 3 * Global_Scale);

      // Foreground output
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      RoundRect(hdc, x, y + brick_half_height, x + Brick_Width * Global_Scale, y + brick_half_height + Global_Scale, 3 * Global_Scale, 3 * Global_Scale);
   }
   else
   {
      SetGraphicsMode(hdc, GM_ADVANCED);

      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)x;
      xform.eDy = (float)y + (float)brick_half_height;

      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Background output
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)Global_Scale;
      back_part_offset = (int)round(offset);
      RoundRect(hdc, 0, -brick_half_height - back_part_offset, Brick_Width * Global_Scale, brick_half_height - back_part_offset, 3 * Global_Scale, 3 * Global_Scale);

      //Foreground output
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      RoundRect(hdc, 0, -brick_half_height, Brick_Width * Global_Scale, brick_half_height, 3 * Global_Scale, 3 * Global_Scale);

      if (rotation_step > 4 && rotation_step <= 12)
      {
         if (letter_type == ELetter_Type::ELT_O)
         {
            SelectObject(hdc, Letter_Pen);
            Ellipse(hdc, 0 + 5 * Global_Scale, (-5 * Global_Scale) / 2, 0 + 10 * Global_Scale, 5 * Global_Scale / 2);
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Draw_Level(HDC hdc)
{// Output all bricks of the level

   for (int i = 0; i < 14; ++i)
      for (int j = 0; j < 12; ++j)
         Draw_Brick(hdc, Level_X_Offset + j * Cell_Width, Level_Y_Offset + i * Cell_Height, (EBrick_Type)Level_01[i][j]);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Draw_Platform(HDC hdc, int x, int y)
{// Draw the platform

   // Draw balls on the sides
   SelectObject(hdc, Platform_Circle_Pen);
   SelectObject(hdc, Platform_Circle_Brush);

   Ellipse(hdc, x * Global_Scale, y * Global_Scale, (x + Circle_Size) * Global_Scale, (y + Circle_Size) * Global_Scale);
   Ellipse(hdc, (x + Inner_Width) * Global_Scale, y * Global_Scale, (x + Circle_Size + Inner_Width) * Global_Scale, (y + Circle_Size) * Global_Scale);
   
   // Draw a highlight
   SelectObject(hdc, Highlight_Pen);
   Arc(hdc, (x + 1) * Global_Scale, (y + 1) * Global_Scale, (x + Circle_Size - 1) * Global_Scale, (y + Circle_Size - 1) * Global_Scale,
      (x + 1 + 1) * Global_Scale, (y + 1) * Global_Scale, (x + 1) * Global_Scale, (y + 1 + 3) * Global_Scale);

   // Draw the middle part
   SelectObject(hdc, Platform_Inner_Pen);
   SelectObject(hdc, Platform_Inner_Brush);

   RoundRect(hdc, (x + 4) * Global_Scale, (y + 1) * Global_Scale, (x + 4 + Inner_Width - 1) * Global_Scale, (y + 1 + 5) * Global_Scale, 3 * Global_Scale, 3 * Global_Scale);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void Draw_Frame(HDC hdc)
{// Drawing the game screen

   Draw_Level(hdc);

   Draw_Platform(hdc, 90, 175);

   for (int i = 0; i < 16; ++i)
   {
      Draw_Brick_Letter(hdc, 650, (Level_Y_Offset + 36) + i * Cell_Height * Global_Scale, i, EBrick_Type::EBT_Red, ELetter_Type::ELT_O);
   }

}
//-----------------------------------------------------------------------------------------------------------------------------------------------


