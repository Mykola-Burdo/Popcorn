#include "Level.h"

//--------------AFalling_Letter--------------------
AFalling_Letter::AFalling_Letter(EBrick_Type brick_type, ELetter_Type letter_type, int x, int y)
   : Brick_Type(brick_type), Letter_Type(letter_type), Got_Hit(false), X(x), Y(y), Rotation_Step(2), Next_Rotation_Tick(AsConfig::Current_Timer_Tick + Ticks_Per_Step)
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

   if (IntersectRect(&intersection_rect, &paint_area, &Letter_Cell))
      Draw_Brick_Letter(hdc);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AFalling_Letter::Act()
{
   Prev_Letter_Cell = Letter_Cell;

   Y += AsConfig::Global_Scale;
   Letter_Cell.top += AsConfig::Global_Scale;
   Letter_Cell.bottom += AsConfig::Global_Scale;

   if(AsConfig::Current_Timer_Tick >= Next_Rotation_Tick)
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
   if(Got_Hit || Letter_Cell.top >= AsConfig::Max_Y_Pos * AsConfig::Global_Scale)
      return true;
   else
      return false;
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
   double rotation_angle; // Pitch to angle conversion
   int brick_half_height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
   int back_part_offset;
   HPEN front_pen, back_pen;
   HBRUSH front_brush, back_brush;
   XFORM xform, old_xform;

   if (!(Brick_Type == EBrick_Type::EBT_Blue || Brick_Type == EBrick_Type::EBT_Red))
      return;// Falling letters can only be from this type of bricks

   // Correction of rotation pitch and angle of rotation
   Rotation_Step %= 16;

   if (Rotation_Step < 8)
      rotation_angle = 2.0 * M_PI / 16.0 * (double)Rotation_Step;
   else
      rotation_angle = 2.0 * M_PI / 16.0 * (double)(8 - Rotation_Step);

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

      RoundRect(hdc, X, Y + brick_half_height - AsConfig::Global_Scale, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

      // Foreground output
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);

      RoundRect(hdc, X, Y + brick_half_height, X + AsConfig::Brick_Width * AsConfig::Global_Scale, Y + brick_half_height + AsConfig::Global_Scale, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);
   }
   else
   {
      // Set up the "flip" matrix of the letter
      xform.eM11 = 1.0f;
      xform.eM12 = 0.0f;
      xform.eM21 = 0.0f;
      xform.eM22 = (float)cos(rotation_angle);
      xform.eDx = (float)X;
      xform.eDy = (float)Y + (float)brick_half_height;

      GetWorldTransform(hdc, &old_xform);
      SetWorldTransform(hdc, &xform);

      // Background output
      SelectObject(hdc, back_pen);
      SelectObject(hdc, back_brush);

      offset = 3.0 * (1.0 - fabs(xform.eM22)) * (double)AsConfig::Global_Scale;
      back_part_offset = (int)round(offset);
      RoundRect(hdc, 0, -brick_half_height - back_part_offset, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height - back_part_offset, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

      //Foreground output
      SelectObject(hdc, front_pen);
      SelectObject(hdc, front_brush);
      RoundRect(hdc, 0, -brick_half_height, AsConfig::Brick_Width * AsConfig::Global_Scale, brick_half_height, 3 * AsConfig::Global_Scale, 3 * AsConfig::Global_Scale);

      if (Rotation_Step > 4 && Rotation_Step <= 12)
      {
         if (Letter_Type == ELetter_Type::ELT_O)
         {
            SelectObject(hdc, AsConfig::Letter_Pen);
            Ellipse(hdc, 0 + 5 * AsConfig::Global_Scale, (-5 * AsConfig::Global_Scale) / 2, 0 + 10 * AsConfig::Global_Scale, 5 * AsConfig::Global_Scale / 2);
         }
      }

      SetWorldTransform(hdc, &old_xform);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




char ALevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
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

char ALevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
{
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

//--------------ALevel--------------------
ALevel::ALevel()
   : Level_Rect{}, Active_Bricks_Count(0), Falling_Letters_Count(0)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool ALevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{// Correcting the position when reflected from bricks

   double direction;
   double min_ball_x, max_ball_x;
   double min_ball_y, max_ball_y;
   int min_level_x, max_level_x;
   int min_level_y, max_level_y;
   bool got_horizontal_hit, got_vertical_hit;
   double horizontal_reflection_pos, vertical_reflection_pos;

   if(next_y_pos + ball->Radius > AsConfig::Level_Y_Offset + (AsConfig::Level_Height - 1) * AsConfig::Cell_Height + AsConfig::Brick_Height)
      return false;

   direction = ball->Get_Direction();

   min_ball_x = next_x_pos - ball->Radius;
   max_ball_x = next_x_pos + ball->Radius;
   min_ball_y = next_y_pos - ball->Radius;
   max_ball_y = next_y_pos + ball->Radius;

   min_level_x = (int)((min_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width);
   max_level_x = (int)((max_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width);
   min_level_y = (int)((min_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);
   max_level_y = (int)((max_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);

   for (int i = max_level_y; i >= min_level_y; --i)
   {
      Current_Brick_Top_Y = AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height;
      Current_Brick_Low_Y = Current_Brick_Top_Y + AsConfig::Brick_Height;

      for (int j = min_level_x; j <= max_level_x; ++j)
      {
         if (Current_Level[i][j] == 0)
            continue;

         Current_Brick_Left_X = AsConfig::Level_X_Offset + j * AsConfig::Cell_Width;
         Current_Brick_Right_X = Current_Brick_Left_X + AsConfig::Brick_Width;

         got_horizontal_hit = Check_Horizontal_Hit(next_x_pos, next_y_pos, j, i, ball, horizontal_reflection_pos);

         got_vertical_hit = Check_Vertical_Hit(next_x_pos, next_y_pos, j, i, ball, vertical_reflection_pos);

         if(got_horizontal_hit && got_vertical_hit)
         {
            if(vertical_reflection_pos < horizontal_reflection_pos)
               ball->Reflect(true);
            else
               ball->Reflect(false);

            On_Hit(j, i);

            return true;
         }
         else
            if(got_horizontal_hit)
            {
               ball->Reflect(false);
               On_Hit(j, i);
               return true;
            }
            else
               if(got_vertical_hit)
               {
                  ball->Reflect(true);
                  On_Hit(j, i);
                  return true;
               }
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Init()
{
   Level_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Level_Rect.top = AsConfig::Level_Y_Offset * AsConfig::Global_Scale;
   Level_Rect.right = Level_Rect.left + AsConfig::Cell_Width * AsConfig::Level_Width * AsConfig::Global_Scale;
   Level_Rect.bottom = Level_Rect.top + AsConfig::Cell_Height * AsConfig::Level_Height * AsConfig::Global_Scale;

   memset(Current_Level, 0, sizeof(Current_Level));
   memset(Active_Bricks, 0, sizeof(Active_Bricks));
   memset(Falling_Letters, 0, sizeof(Falling_Letters));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width])
{
   memcpy(Current_Level, level, sizeof(Current_Level));
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Act()
{

   Act_Objects((AGraphics_Object **)&Active_Bricks, AsConfig::Max_Active_Bricks_Count);

   Act_Objects((AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Draw(HDC hdc, RECT& paint_area)
{// Output all bricks of the level

   RECT intersection_rect, brick_rect;

   if (IntersectRect(&intersection_rect, &paint_area, &Level_Rect))
   {
      for (int i = 0; i < AsConfig::Level_Height; ++i)
         for (int j = 0; j < AsConfig::Level_Width; ++j)
         {
            brick_rect.left = (AsConfig::Level_X_Offset + j * AsConfig::Cell_Width) * AsConfig::Global_Scale;
            brick_rect.top = (AsConfig::Level_Y_Offset + i * AsConfig::Cell_Height) * AsConfig::Global_Scale;
            brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
            brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

            if (IntersectRect(&intersection_rect, &paint_area, &brick_rect))
               Draw_Brick(hdc, brick_rect, (EBrick_Type)Current_Level[i][j]);
         }

      Draw_Objects(hdc, paint_area, (AGraphics_Object **)&Active_Bricks, AsConfig::Max_Active_Bricks_Count);
   }

   Draw_Objects(hdc, paint_area, (AGraphics_Object **)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::On_Hit(int brick_x, int brick_y)
{
   EBrick_Type brick_type;

   brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

   if(Add_Falling_Letter(brick_x, brick_y, brick_type))
      Current_Level[brick_y][brick_x] = (char)EBrick_Type::EBT_None;
   else
      Add_Active_Brick(brick_x, brick_y, brick_type);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool ALevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{// Create a falling letter if we can

   int letter_x, letter_y;

   //ELetter_Type letter_type;
   AFalling_Letter* falling_letter;

   if (brick_type == EBrick_Type::EBT_Red || brick_type == EBrick_Type::EBT_Blue)
   {
      if (AsConfig::Rand(AsConfig::Hits_Per_Letter) == 0)
      {
         if (Falling_Letters_Count < AsConfig::Max_Falling_Letters_Count)
         {
            //letter_type = ELetter_Type::ELT_O;

            for (int i = 0; i < AsConfig::Max_Falling_Letters_Count; ++i)
            {
               if (Falling_Letters[i] == 0)
               {
                  letter_x = (brick_x * AsConfig::Cell_Width + AsConfig::Level_X_Offset) * AsConfig::Global_Scale;
                  letter_y = (brick_y * AsConfig::Cell_Height + AsConfig::Level_Y_Offset) * AsConfig::Global_Scale;

                  falling_letter = new AFalling_Letter(brick_type, ELetter_Type::ELT_O, letter_x, letter_y);
                  Falling_Letters[i] = falling_letter;
                  ++Falling_Letters_Count;
                  break;
               }
            }

            return true;
         }
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Add_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type)
{// Create an active brick if we can

   AActive_Brick *active_brick;

   if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
      return; // There are too many active bricks!

   switch (brick_type)
   {
   case EBrick_Type::EBT_None:
      return;

   case EBrick_Type::EBT_Red:
   case EBrick_Type::EBT_Blue:
      active_brick = new AActive_Brick(brick_type, brick_x, brick_y);
      Current_Level[brick_y][brick_x] = (char)EBrick_Type::EBT_None;
      break;
/*
   case EBrick_Type::EBT_Unbreakable:
      break;

   case EBrick_Type::EBT_Multihit_1:
      break;

   case EBrick_Type::EBT_Multihit_2:
      break;

   case EBrick_Type::EBT_Multihit_3:
      break;

   case EBrick_Type::EBT_Multihit_4:
      break;

   case EBrick_Type::EBT_Parachute:
      break;

   case EBrick_Type::EBT_Teleport:
      break;

   case EBrick_Type::EBT_Ad:
      break;
*/
   default:
      return;

   }

   // Adding a new active brick to the first free space
   for(int i = 0; i < AsConfig::Max_Active_Bricks_Count; ++i)
   {
      if(Active_Bricks[i] == 0)
      {
         Active_Bricks[i] = active_brick;
         ++Active_Bricks_Count;
         break;
      }
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool ALevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflection_pos)
{
   double direction = ball->Get_Direction();

   if (ball->Is_Moving_Up())
   {// Checking if it hits the bottom

      if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Low_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
      {
         // Checking the possibility of a bounce down
         if(level_y < AsConfig::Level_Height - 1 && Current_Level[level_y + 1][level_x] == 0)
            return true;
         else
            return false;
      }
   }
   else
   {// Checking if it hits the top

      if (Hit_Circle_On_Line(next_y_pos - Current_Brick_Top_Y, next_x_pos, Current_Brick_Left_X, Current_Brick_Right_X, ball->Radius, reflection_pos))
      {
         // Checking the possibility of a bounce up
         if (level_y > 0 && Current_Level[level_y - 1][level_x] == 0)
            return true;
         else
            return false;
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool ALevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double &reflection_pos)
{
   double direction = ball->Get_Direction();

   if (!ball->Is_Moving_Left())
   {// Checking the hit on the left side

      if (Hit_Circle_On_Line(Current_Brick_Left_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
      {
         // Checking the possibility of a rebound to the left
         if (level_x > 0 && Current_Level[level_y][level_x - 1] == 0)
            return true;
         else
            return false;
      }
   }
   else
   {// Checking the hit on the right side

      if (Hit_Circle_On_Line(Current_Brick_Right_X - next_x_pos, next_y_pos, Current_Brick_Top_Y, Current_Brick_Low_Y, ball->Radius, reflection_pos))
      {
         // Checking the possibility of a rebound to the right
         if (level_x < AsConfig::Level_Width - 1 && Current_Level[level_y][level_x + 1] == 0)
            return true;
         else
            return false;
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Draw_Brick(HDC hdc, RECT &brick_rect, EBrick_Type brick_type)
{// The conclusion of the "brick"

   HPEN pen;
   HBRUSH brush;

   switch (brick_type)
   {
   case EBrick_Type::EBT_None:
      pen = AsConfig::BG_Pen;
      brush = AsConfig::BG_Brush;
      return;

   case EBrick_Type::EBT_Red:
      pen = AsConfig::Brick_Red_Pen;
      brush = AsConfig::Brick_Red_Brush;
      break;

   case EBrick_Type::EBT_Blue:
      pen = AsConfig::Brick_Blue_Pen;
      brush = AsConfig::Brick_Blue_Brush;
      break;

   default:
      return;
   }

   SelectObject(hdc, pen);
   SelectObject(hdc, brush);
   RoundRect(hdc, brick_rect.left, brick_rect.top, brick_rect.right - 1, brick_rect.bottom - 1, 2 * AsConfig::Global_Scale, 2 * AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Draw_Objects(HDC hdc, RECT& paint_area, AGraphics_Object** objects_array, int objects_max_count)
{
   for (int i = 0; i < objects_max_count; ++i)
   {
      if (objects_array[i] != 0)
         objects_array[i]->Draw(hdc, paint_area);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALevel::Act_Objects(AGraphics_Object** objects_array, int objects_max_count)
{
   for (int i = 0; i < objects_max_count; ++i)
   {
      if (objects_array[i] != 0)
      {
         objects_array[i]->Act();

         if (objects_array[i]->Is_Finished())
         {
            delete objects_array[i];
            objects_array[i] = 0;
            --Falling_Letters_Count;
         }
      }
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------