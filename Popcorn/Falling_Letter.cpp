#include "Falling_Letter.h"

//--------------AFalling_Letter--------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
   : Brick_Type(brick_type), Letter_Type(letter_type), Falling_Letter_State(EFalling_Letter_State::EFLS_Normal), X(x), Y(y), Rotation_Step(2),
   Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step)
{
   Letter_Cell.left = X;
   Letter_Cell.top = Y;
   Letter_Cell.right = Letter_Cell.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   Letter_Cell.bottom = Letter_Cell.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   Prev_Letter_Cell = Letter_Cell;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Draw(HDC hdc, RECT& paint_area)
{
   RECT intersection_rect;

   // Background cleaning
   if (IntersectRect(&intersection_rect, &paint_area, &Prev_Letter_Cell))
   {
      SelectObject(hdc, AsConfig::BG_Pen);
      SelectObject(hdc, AsConfig::BG_Brush);

      Rectangle(hdc, Prev_Letter_Cell.left, Prev_Letter_Cell.top, Prev_Letter_Cell.right, Prev_Letter_Cell.bottom);
   }

   if(Falling_Letter_State == EFalling_Letter_State::EFLS_Finalizing)
   {
      Falling_Letter_State = EFalling_Letter_State::EFLS_Finished;
      return;
   }

   if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
      Draw_Brick_Letter(hdc);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Act()
{
   if(Falling_Letter_State != EFalling_Letter_State::EFLS_Normal)
      return;

   if(Letter_Cell.top >= AsConfig::Max_Y_Pos * AsConfig::Global_Scale)
   {
      Finalize();
      return;
   }

   Prev_Letter_Cell = Letter_Cell;

   Y += AsConfig::Global_Scale;
   Letter_Cell.top += AsConfig::Global_Scale;
   Letter_Cell.bottom += AsConfig::Global_Scale;

   if (AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
   {
      ++Rotation_Step;
      Next_Rotation_Tick += Ticks_Per_Step;
   }

   InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AFalling_Letter::Is_Finished()
{
   if(Falling_Letter_State == EFalling_Letter_State::EFLS_Finished)
      return true;
   else
      return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Get_Letter_Cell(RECT &rect)
{
   rect = Letter_Cell;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Finalize()
{
   Falling_Letter_State = EFalling_Letter_State::EFLS_Finalizing;

   InvalidateRect(AsConfig::Hwnd, &Prev_Letter_Cell, FALSE);
   InvalidateRect(AsConfig::Hwnd, &Letter_Cell, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Test_Draw_All_Steps(HDC hdc)
{
   int x_step = AsConfig::Cell_Width * AsConfig::Global_Scale;

   Rotation_Step = 0;

   for(int i = 0; i < Max_Rotation_Step; ++i)
   {
      Draw_Brick_Letter(hdc);
      ++Rotation_Step;

      X += x_step;
      Letter_Cell.left += x_step;
      Letter_Cell.right += x_step;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Set_Brick_Letter_Colors(bool is_switch_color, HPEN& front_pen, HBRUSH& front_brush, HPEN& back_pen, HBRUSH& back_brush)
{
   if (is_switch_color)
   {
      front_pen = AsConfig::Brick_Red_Pen;
      front_brush = AsConfig::Brick_Red_Brush;

      back_pen = AsConfig::Brick_Blue_Pen;
      back_brush = AsConfig::Brick_Blue_Brush;
   }
   else
   {
      front_pen = AsConfig::Brick_Blue_Pen;
      front_brush = AsConfig::Brick_Blue_Brush;

      back_pen = AsConfig::Brick_Red_Pen;
      back_brush = AsConfig::Brick_Red_Brush;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Draw_Brick_Letter(HDC hdc)
{// Falling letter output

   bool switch_color;
   double offset;
   double y_ratio;
   double rotation_angle; // Pitch to angle conversion
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(Brick_Type == EBrick_Type::EBT_Blue || Brick_Type == EBrick_Type::EBT_Red))
      return;// Falling letters can only be from this type of bricks

   // Correction of rotation pitch and angle of rotation
   Rotation_Step %= Max_Rotation_Step;

   if (Rotation_Step < 8)
      rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Step * (double)Rotation_Step;
   else
      rotation_angle = 2.0 * M_PI / (double)Max_Rotation_Step * (double)(8 - Rotation_Step);

   if (Rotation_Step > 4 && Rotation_Step <= 12)
   {
      if (Brick_Type == EBrick_Type::EBT_Blue)
         switch_color = true;
      else
         switch_color = false;
   }
   else
   {
      if (Brick_Type == EBrick_Type::EBT_Red)
         switch_color = true;
      else
         switch_color = false;
   }

   Set_Brick_Letter_Colors(switch_color, front_pen, front_brush, back_pen, back_brush);

   if (Rotation_Step == 4 || Rotation_Step == 12)
   {
      // Background output
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      RoundRect(hdc, X, Y + Brick_Half_Height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + Brick_Half_Height, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

      // Foreground output
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      RoundRect(hdc, X, Y + Brick_Half_Height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + Brick_Half_Height + AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);
   }
   else
   {
      // Set up the "flip" matrix of the letter
      y_ratio = cos(rotation_angle);

      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)fabs(y_ratio);
      xform.eDx = (float)X;
      xform.eDy = (float)Y + (float)Brick_Half_Height;

      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Background output
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
      back_part_offset = (int)round(offset);

      if(y_ratio < 0.0)
         back_part_offset = -back_part_offset;

      RoundRect(hdc, 0, -Brick_Half_Height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Brick_Half_Height - back_part_offset, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

      //Foreground output
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      RoundRect(hdc, 0, -Brick_Half_Height, AsConfig::Brick_Width * AsConfig::Global_Scale - 1, Brick_Half_Height, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

      if (Rotation_Step > 4 && Rotation_Step <= 12)
      {
         SelectObject(hdc, AsConfig::Letter_Pen);
         switch (Letter_Type)
         {
         case ELetter_Type::ELT_O:
            Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, 1 * AsConfig::Global_Scale - Brick_Half_Height, 0 + 10 * AsConfig::Global_Scale, 6 * AsConfig::Global_Scale - Brick_Half_Height - 1);
            break;

         case ELetter_Type::ELT_I:
            Draw_Line(hdc, 5, 1, 5, 6);
            Draw_Line(hdc, 5, 6, 9, 1);
            Draw_Line(hdc, 9, 1, 9, 6);
            break;

         case ELetter_Type::ELT_G:
            Draw_Line(hdc, 7, 1, 7, 6);
            Draw_Line(hdc, 5, 3, 9, 3);
            Draw_Line(hdc, 4, 1, 5, 3);
            Draw_Line(hdc, 5, 3, 4, 6);
            Draw_Line(hdc, 10, 1, 9, 3);
            Draw_Line(hdc, 9, 3, 10, 6);
            break;
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Draw_Line(HDC hdc, int x_1, int y_1, int x_2, int y_2)
{
   int start_y, end_y;

   start_y = y_1 * AsConfig::Global_Scale - Brick_Half_Height;

   if(y_1 == 6)
      --start_y;

   end_y = y_2 * AsConfig::Global_Scale - Brick_Half_Height;

   if (y_2 == 6)
      --end_y;

   MoveToEx(hdc, x_1 * AsConfig::Global_Scale + 1, start_y, 0);
   LineTo(hdc, x_2 * AsConfig::Global_Scale + 1, end_y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
