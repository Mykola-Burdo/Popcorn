#include "Border.h"

//--------------AsBorder--------------------
AsBorder::AsBorder()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBorder::Draw(HDC hdc, RECT& paint_area)
{// Draws a level border

   // Draw the left border
   for (int i = 0; i < 50; ++i)
      Draw_Element(hdc, 2, 1 + i * 4, false);

   // Draw the right border
   for (int i = 0; i < 50; ++i)
      Draw_Element(hdc, 201, 1 + i * 4, false);

   // Draw the top border
   for (int i = 0; i < 50; ++i)
      Draw_Element(hdc, 3 + i * 4, 0, true);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
   bool got_hit = false;

   // Frame bounce position correction
   if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
   {
      got_hit = true;
      ball->Reflect(false);
   }

   if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
   {
      got_hit = true;
      ball->Reflect(true);
   }

   if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos + 1)
   {
      got_hit = true;
      ball->Reflect(false);
   }

   if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos)
   {
      if (AsConfig::Level_Has_Floor)
      {
         got_hit = true;
         ball->Reflect(true);
      }
      else
      {
         if (next_y_pos + ball->Radius > (double)AsConfig::Max_Y_Pos + ball->Radius * 4.0) // In order for the ball to fly below the floor, we check its max_y_pos below the visible border
            ball->Set_State(EBall_State::EBS_Lost, next_x_pos);
      }
   }

   return got_hit;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBorder::Draw_Element(HDC hdc, int x, int y, bool is_top_border)
{// Draws a level border element

   // Main line
   AsConfig::Blue_Color.Select(hdc);

   if (is_top_border)
      Rectangle(hdc, x * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);
   else
      Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

   // White border
   AsConfig::White_Color.Select(hdc);

   if (is_top_border)
      Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale - 1, (y + 1) * AsConfig::Global_Scale - 1);
   else
      Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale - 1, (y + 4) * AsConfig::Global_Scale - 1);

   // Perforation
   AsConfig::BG_Color.Select(hdc);

   if (is_top_border)
      Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 3) * AsConfig::Global_Scale - 1);
   else
      Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale - 1, (y + 2) * AsConfig::Global_Scale - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------