#include "Border.h"

//--------------AsBorder--------------------
AsBorder::AsBorder()
{
   Floor_Rect.left = AsConfig::Level_X_Offset * AsConfig::Global_Scale;
   Floor_Rect.top = AsConfig::Floor_Y_Pos * AsConfig::Global_Scale;
   Floor_Rect.right = (AsConfig::Max_X_Pos - 1) * AsConfig::Global_Scale;
   Floor_Rect.bottom = AsConfig::Max_Y_Pos * AsConfig::Global_Scale;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBorder::Draw(HDC hdc, RECT &paint_area)
{// Draws a level border

   // Draw the left border
   for (int i = 0; i < 50; ++i)
      Draw_Element(hdc, paint_area, 2, 1 + i * 4, false);

   // Draw the right border
   for (int i = 0; i < 50; ++i)
      Draw_Element(hdc, paint_area, AsConfig::Max_X_Pos + 1, 1 + i * 4, false);

   // Draw the top border
   for (int i = 0; i < 50; ++i)
      Draw_Element(hdc, paint_area, 3 + i * 4, 0, true);

   // Floor (if any)
   if (AsConfig::Level_Has_Floor)
      Draw_Floor(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBorder::Redraw_Floor()
{
   InvalidateRect(AsConfig::Hwnd, &Floor_Rect, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{// Frame bounce position correction

   bool got_hit = false;

   // Left edge
   if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
   {
      got_hit = true;
      ball->Reflect(false);
   }

   // Top edge
   if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
   {
      got_hit = true;
      ball->Reflect(true);
   }

   // Right edge
   if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos + 1)
   {
      got_hit = true;
      ball->Reflect(false);
   }

   // Bottom edge
   if (AsConfig::Level_Has_Floor && next_y_pos + ball->Radius > AsConfig::Floor_Y_Pos)
   {

      got_hit = true;
      ball->Reflect(true);
   }

   // In order for the ball to fly below the floor, we check its max_y_pos below the visible border
   if (next_y_pos + ball->Radius > (double)AsConfig::Max_Y_Pos + ball->Radius * 4.0)
      ball->Set_State(EBall_State::EBS_Lost);

   return got_hit;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBorder::Draw_Element(HDC hdc, RECT &paint_area, int x, int y, bool is_top_border)
{// Draws a level border element

   RECT intersection_rect, rect;

   rect.left = x * AsConfig::Global_Scale;
   rect.top = (y + 1) * AsConfig::Global_Scale;
   rect.right = (x + 4) * AsConfig::Global_Scale;
   rect.bottom = (y + 4) * AsConfig::Global_Scale;

   if (!IntersectRect(&intersection_rect, &paint_area, &rect))
      return;

   // Main line
   AsConfig::Blue_Color.Select(hdc);

   if (is_top_border)
      Rectangle(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);
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

void AsBorder::Draw_Floor(HDC hdc, RECT &paint_area)
{
   int stroked_count;
   int x_pos, y_pos;
   int line_len = 4 * AsConfig::Global_Scale;
   int gap_len = 2 * AsConfig::Global_Scale;
   int stroke_len = line_len + gap_len;
   RECT intersection_rect;

   if (!IntersectRect(&intersection_rect, &paint_area, &Floor_Rect))
      return;

   stroked_count = (Floor_Rect.right - Floor_Rect.left + AsConfig::Global_Scale) / stroke_len;

   x_pos = Floor_Rect.left + 1;
   y_pos = Floor_Rect.top + (Floor_Rect.bottom - Floor_Rect.top) / 2;

   AsConfig::Letter_Color.Select(hdc);

   for(int i = 0; i < stroked_count; ++i)
   {

      MoveToEx(hdc, x_pos, y_pos, 0);
      LineTo(hdc, x_pos + line_len, y_pos);

      x_pos += stroke_len;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------