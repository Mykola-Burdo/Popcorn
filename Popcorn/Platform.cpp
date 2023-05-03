#include "Platform.h"

//--------------AsPlatform--------------------
AsPlatform::~AsPlatform()
{
   delete[] Normal_Platform_Image;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsPlatform::AsPlatform()
   : X_Pos(AsConfig::Border_X_Offset), Normal_Platform_Image_Width(0), Normal_Platform_Image_Height(0), Normal_Platform_Image(0), 
   Inner_Width(AsConfig::Platform_Normal_Inner_Width), Rolling_Step(0), Last_Redraw_timer_Tick(0), Speed(0.0), Ball_Set(0), 
   Platform_Glue(Platform_State), Platform_Expanding(Platform_State), Platform_Laser(Platform_State), Left_Key_Down(false), Right_Key_Down(false), 
   Platform_Rect{}, Prev_Platform_Rect{}, Highlight_Color(255, 255, 255), Platform_Circle_Color(151, 0, 0), Platform_Inner_Color(0, 128, 192)
{
   X_Pos = (AsConfig::Max_X_Pos - AsConfig::Platform_Normal_Width) / 2;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
   double inner_left_x, inner_right_x;
   double inner_top_y, inner_low_y;
   double inner_y;
   double reflection_pos;
   double ball_x, ball_y;

   // Correcting the position when reflected from the platform
   if (next_y_pos + ball->Radius < AsConfig::Platform_Y_Pos)
      return false;

   inner_top_y = (double)(AsConfig::Platform_Y_Pos + 1);
   inner_low_y = (double)(AsConfig::Platform_Y_Pos + AsConfig::Platform_Height - 1);
   inner_left_x = (double)(X_Pos + AsConfig::Platform_Circle_Size - 1);
   inner_right_x = (double)(X_Pos + Get_Current_Width() - (AsConfig::Platform_Circle_Size - 1));

   // Checking the reflection from the side balls
   if(Reflect_On_Circle(next_x_pos, next_y_pos, 0.0, ball))
      goto _on_hit; // From left

   if (Reflect_On_Circle(next_x_pos, next_y_pos, Get_Current_Width() - AsConfig::Platform_Circle_Size, ball))
      goto _on_hit; // From the right

   // Checking the reflection from the central part of the platform
   if(ball->Is_Moving_Up())
      inner_y = inner_low_y; // From the bottom edge
   else
      inner_y = inner_top_y; // From top edge

   if (Hit_Circle_On_Line(next_y_pos - inner_y, next_x_pos, inner_left_x, inner_right_x, ball->Radius, reflection_pos))
   {
      ball->Reflect(true);
      goto _on_hit;
   }

   return false;

_on_hit:
   if(ball->Get_State() == EBall_State::EBS_On_Parachute)
      ball->Set_State(EBall_State::EBS_Off_Parachute);

   if (Platform_State == EPlatform_State::Glue && Platform_State.Glue == EPlatform_Transformation::Active)
   {
      ball->Get_Center(ball_x, ball_y);
      ball->Set_State(EBall_State::EBS_On_Platform, ball_x, ball_y);
   }

   return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Begin_Movement()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Finish_Movement()
{
   if(Platform_State.Moving == EPlatform_Moving_State::Stop)
      return;

   Redraw_Platform();

   if(Platform_State.Moving == EPlatform_Moving_State::Stopping)
      Platform_State.Moving = EPlatform_Moving_State::Stop;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Advance(double max_speed)
{
   double next_step;

   if(Platform_State.Moving == EPlatform_Moving_State::Stopping || Platform_State.Moving == EPlatform_Moving_State::Stop)
      return;

   next_step = Speed / max_speed * AsConfig::Moving_Step_Size;

   X_Pos += next_step;

   if(Correct_Platform_Pos())
   {
      Speed = 0.0;
      Platform_State.Moving = EPlatform_Moving_State::Stopping;
   }

   // Offset glued balls
   if((Platform_State == EPlatform_State::Regular && Platform_State.Regular == EPlatform_Substate_Regular::Ready)
      || (Platform_State == EPlatform_State::Glue && Platform_State.Glue == EPlatform_Transformation::Active))
   {
      if(Platform_State.Moving == EPlatform_Moving_State::Moving_Left)
         Ball_Set->On_Platform_Advance(M_PI, fabs(Speed), max_speed);
      else
         if(Platform_State.Moving == EPlatform_Moving_State::Moving_Right)
            Ball_Set->On_Platform_Advance(0.0, fabs(Speed), max_speed);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double AsPlatform::Get_Speed()
{
   return Speed;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Act()
{
   bool correct_pos;
   EPlatform_State next_state;

   switch (Platform_State)
   {
   case EPlatform_State::Meltdown:
      Act_For_Meltdown_State();
      break;


   case EPlatform_State::Rolling:
      Act_For_Rolling_State();
      break;


   case EPlatform_State::Glue:
      if(Platform_Glue.Act(Ball_Set, next_state))
         Redraw_Platform();

      if(next_state != EPlatform_State::Unknown)
         Set_State(next_state);
      break;


   case EPlatform_State::Expanding:
      if(Platform_Expanding.Act(X_Pos, next_state, correct_pos))
         Redraw_Platform();

      if(correct_pos)
         Correct_Platform_Pos();

      if(next_state != EPlatform_State::Unknown)
         Set_State(next_state);
      break;


   case EPlatform_State::Laser:
      if(Platform_Laser.Act(next_state, X_Pos))
         Redraw_Platform();

      if(next_state != EPlatform_State::Unknown)
         Set_State(next_state);
      break;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Clear(HDC hdc, RECT &paint_area)
{
   RECT intersection_rect;

   if (!IntersectRect(&intersection_rect, &paint_area, &Prev_Platform_Rect))
      return;

   switch (Platform_State)
   {
   case EPlatform_State::Regular:
   {
      if(Platform_State.Regular == EPlatform_Substate_Regular::Missing)
         break;
   }
   // else - no break!

   case EPlatform_State::Rolling:
   case EPlatform_State::Glue:
   case EPlatform_State::Expanding:
   case EPlatform_State::Laser:
      // Cleaning up the background
      AsConfig::BG_Color.Select(hdc);
      Rectangle(hdc, Prev_Platform_Rect.left, Prev_Platform_Rect.top, Prev_Platform_Rect.right, Prev_Platform_Rect.bottom);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw(HDC hdc, RECT &paint_area)
{// Draw the platform

   RECT intersection_rect;

   if (!IntersectRect(&intersection_rect, &paint_area, &Platform_Rect))
      return;

   switch (Platform_State)
   {
   case EPlatform_State::Regular:
      if(Platform_State.Regular == EPlatform_Substate_Regular::Ready || Platform_State.Regular == EPlatform_Substate_Regular::Normal)
         Draw_Normal_State(hdc, paint_area);
      break;

   case EPlatform_State::Meltdown:
      if(Platform_State.Meltdown == EPlatform_Substate_Meltdown::Active)
         Draw_Meltdown_State(hdc, paint_area);
      break;

   case EPlatform_State::Rolling:
      Draw_Rolling_State(hdc, paint_area);
      break;

   case EPlatform_State::Glue:
      Draw_Normal_State(hdc, paint_area);
      Platform_Glue.Draw_State(hdc, X_Pos);
      break;

   case EPlatform_State::Expanding:
      Platform_Expanding.Draw_State(hdc, X_Pos);
      break;

   case EPlatform_State::Laser:
      Platform_Laser.Draw_State(hdc, X_Pos, Platform_Rect);
      break;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Is_Finished()
{
   return false; // Stub because this method is not used
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Init(AsBall_Set *ball_set, AsLaser_Beam_Set *laser_beam_set)
{
   Ball_Set = ball_set;

   Platform_Expanding.Init(Highlight_Color, Platform_Circle_Color, Platform_Inner_Color);
   Platform_Laser.Init(laser_beam_set, Highlight_Color, Platform_Circle_Color, Platform_Inner_Color);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

EPlatform_State AsPlatform::Get_State()
{
   return Platform_State;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Set_State(EPlatform_State new_state)
{
   int len;

   if(Platform_State == new_state)
      return;

   switch(new_state)
   {
   case EPlatform_State::Regular:
      AsConfig::Throw(); // Regular state is set implicitly when called Set_State(EPlatform_Substate_Regular new_regular_state)
      break;

   case EPlatform_State::Meltdown:
      if(Platform_State != EPlatform_State::Regular)
      {
         Platform_State.Set_Next_State(new_state);
         return;
      }

      Speed = 0.0;
      Platform_State.Meltdown = EPlatform_Substate_Meltdown::Init;

      len = sizeof(Meltdown_Platform_Y_Pos) / sizeof(Meltdown_Platform_Y_Pos[0]);

      for (int i = 0; i < len; ++i)
         Meltdown_Platform_Y_Pos[i] = Platform_Rect.top;

      break;

   case EPlatform_State::Rolling:
      Platform_State.Rolling = EPlatform_Substate_Rolling::Roll_In;
      X_Pos = AsConfig::Max_X_Pos - 1;
      Rolling_Step = Max_Rolling_Step - 1;
      break;

   case EPlatform_State::Glue:
      if(Set_Transformation_State(new_state, Platform_State.Glue))
         return;
      else
         Platform_Glue.Reset();

      break;

   case EPlatform_State::Expanding:
      if(Set_Transformation_State(new_state, Platform_State.Expanding))
         return;
      else
         Platform_Expanding.Reset();

      break;

   case EPlatform_State::Laser:
      if(Set_Transformation_State(new_state, Platform_State.Laser))
         return;
      else
         Platform_Laser.Reset();

      break;
   }
      
   Platform_State = new_state;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Set_State(EPlatform_Substate_Regular new_regular_state)
{
   EPlatform_State next_state;

   next_state = Platform_State.Set_State(new_regular_state);

   if(next_state != EPlatform_State::Unknown)
      Set_State(next_state);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Has_State(EPlatform_Substate_Regular regular_state)
{
   if(Platform_State != EPlatform_State::Regular)
      return false;

   if(Platform_State.Regular == regular_state)
      return true;
   else 
      return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Redraw_Platform()
{
   if(Last_Redraw_timer_Tick != AsConfig::Current_Timer_Tick)
   {
      Prev_Platform_Rect = Platform_Rect;
      Last_Redraw_timer_Tick = AsConfig::Current_Timer_Tick;
   }

   Platform_Rect.left = (int)(X_Pos * AsConfig::D_Global_Scale);
   Platform_Rect.top = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   Platform_Rect.right = (int)((X_Pos + Get_Current_Width()) * AsConfig::D_Global_Scale);
   Platform_Rect.bottom = Platform_Rect.top + AsConfig::Platform_Height * AsConfig::Global_Scale;


   if(Platform_State == EPlatform_State::Meltdown)
      Prev_Platform_Rect.bottom = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   AsConfig::Invalidate_Rect(Prev_Platform_Rect);
   AsConfig::Invalidate_Rect(Platform_Rect);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Move(bool to_left, bool key_down)
{
   if(! (Has_State(EPlatform_Substate_Regular::Normal) || Platform_State == EPlatform_State::Glue || Platform_State == EPlatform_State::Expanding
       || Platform_State == EPlatform_State::Laser))
      return;

   if (to_left)
      Left_Key_Down = key_down;
   else
      Right_Key_Down = key_down;

   if(Left_Key_Down && Right_Key_Down)
      return; // Simultaneous pressing of two keys is ignored

   if (!Left_Key_Down && !Right_Key_Down)
   {
      Speed = 0.0;
      Platform_State.Moving = EPlatform_Moving_State::Stopping;
      return;
   }

   if(Left_Key_Down)
   {
      Platform_State.Moving = EPlatform_Moving_State::Moving_Left;
      Speed = -X_Step;
   }
   
   if(Right_Key_Down)
   {
      Platform_State.Moving = EPlatform_Moving_State::Moving_Right;
      Speed = X_Step;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::On_Space_Key(bool key_down)
{
   if(Has_State(EPlatform_Substate_Regular::Ready))
   {
      if (!key_down)
      {
         Ball_Set->Release_From_Platform(Get_Middle_Pos());
         Set_State(EPlatform_Substate_Regular::Normal);
      }
   }
   else
      if(Platform_State == EPlatform_State::Glue)
      {
         if (!key_down)
            Ball_Set->Release_Next_Ball();
      }
      else if(Platform_State == EPlatform_State::Laser)
         Platform_Laser.Fire(key_down);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Hit_By(AFalling_Letter *falling_letter)
{
   RECT intersection_rect, falling_letter_rect;

   falling_letter->Get_Letter_Cell(falling_letter_rect);

   if (IntersectRect(&intersection_rect, &falling_letter_rect, &Platform_Rect))
      return true;
   else
      return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double AsPlatform::Get_Middle_Pos()
{
   return X_Pos + Get_Current_Width() / 2.0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Set_Transformation_State(EPlatform_State new_state, EPlatform_Transformation &transformation_state)
{
   if(Platform_State != EPlatform_State::Regular)
   {
      Platform_State.Set_Next_State(new_state);
      return true;
   }

   if(transformation_state == EPlatform_Transformation::Finalize)
      return true;
   else
   {
      transformation_state = EPlatform_Transformation::Init;
      return false;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Act_For_Meltdown_State()
{
   switch(Platform_State.Meltdown)
   {
   case EPlatform_Substate_Meltdown::Init:
      Platform_State.Meltdown = EPlatform_Substate_Meltdown::Active;
      break;

   case EPlatform_Substate_Meltdown::Active:
      Redraw_Platform();
      break;
   }
      
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Act_For_Rolling_State()
{
   switch (Platform_State.Rolling)
   {
   case EPlatform_Substate_Rolling::Roll_In:
      ++Rolling_Step;

      if(Rolling_Step >= Max_Rolling_Step)
         Rolling_Step -= Max_Rolling_Step;

      X_Pos -= Rolling_Platform_Speed;

      if (X_Pos <= Roll_In_Platform_End_X_Pos)
      {
         X_Pos = Roll_In_Platform_End_X_Pos;
         Platform_State.Rolling = EPlatform_Substate_Rolling::Expand_Roll_In;
         Inner_Width = 1;
      }
      break;

   case EPlatform_Substate_Rolling::Expand_Roll_In:
      --X_Pos;
      Inner_Width += 2;
      if(Inner_Width >= AsConfig::Platform_Normal_Inner_Width)
      {
         Inner_Width = AsConfig::Platform_Normal_Inner_Width;
         Set_State(EPlatform_Substate_Regular::Ready);
         Platform_State.Rolling = EPlatform_Substate_Rolling::Unknown;
         Redraw_Platform();
      }
      break;
   }

   Redraw_Platform();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw_Normal_State(HDC hdc, RECT &paint_area)
{// Draw the platform in the normal state

   const int scale = AsConfig::Global_Scale;
   const double d_scale = AsConfig::D_Global_Scale;
   double x = X_Pos;
   int y = AsConfig::Platform_Y_Pos;
   RECT inner_rect, rect;

   // Draw balls on the sides
   Platform_Circle_Color.Select(hdc);

   rect.left = (int)(x * d_scale);
   rect.top = y * scale;
   rect.right = (int)((x + (double)AsConfig::Platform_Circle_Size) * d_scale);
   rect.bottom = (y + AsConfig::Platform_Circle_Size) * scale;
   Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

   rect.left = (int)((x + Inner_Width) * d_scale);
   rect.top = y * scale;
   rect.right = (int)((x + (double)AsConfig::Platform_Circle_Size + Inner_Width) * d_scale);
   rect.bottom = (y + AsConfig::Platform_Circle_Size) * scale;
   Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

   // Draw a highlight
   Platform_Expanding.Draw_Circle_Highlight(hdc, (int)(x * d_scale), y * scale);

   // Draw the middle part
   Platform_Inner_Color.Select(hdc);

   inner_rect.left = (int)((x + 4) * d_scale);
   inner_rect.top = (y + 1) * scale;
   inner_rect.right = (int)((x + 4 + Inner_Width - 1) * d_scale);
   inner_rect.bottom = (y + 1 + 5) * scale;

   AsConfig::Round_Rect(hdc, inner_rect, 3);

   if (Normal_Platform_Image == 0 && Has_State(EPlatform_Substate_Regular::Ready))
      Get_Normal_Platform_Image(hdc);

}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw_Meltdown_State(HDC hdc, RECT &paint_area)
{// Drawing a platform in a molten state

   int i, j;
   int x, y;
   int y_offset;
   int stroke_len;
   int moved_columns_count = 0;
   int max_platform_y;
   const AColor *color;

   max_platform_y = (AsConfig::Max_Y_Pos + 1) * AsConfig::Global_Scale;

   for(i = 0; i < Normal_Platform_Image_Width; ++i)
   {
      if(Meltdown_Platform_Y_Pos[i] > max_platform_y)
         continue;

      ++moved_columns_count;

      y_offset = AsConfig::Rand(Meltdown_Speed) + 1;
      x = Platform_Rect.left + i;

      j = 0;

      y = Meltdown_Platform_Y_Pos[i];

      MoveToEx(hdc, x, y, 0);
      
      // Draw a sequence of vertical strokes of different colors (according to the prototype saved in Normal_Platform_Image)
      while(Get_Platform_Image_Stroke_Color(i, j, &color, stroke_len))
      {
         color->Select_Pen(hdc);
         LineTo(hdc, x, y + stroke_len);

         y += stroke_len;
         j += stroke_len;
      }

      // Erase the background pixels above the stroke
      y = Meltdown_Platform_Y_Pos[i];
      MoveToEx(hdc, x, y, 0);
      AsConfig::BG_Color.Select_Pen(hdc);
      LineTo(hdc, x, y + y_offset);

      Meltdown_Platform_Y_Pos[i] += y_offset;
   }

   if (moved_columns_count == 0)
      Set_State(EPlatform_Substate_Regular::Missing); // The entire platform has moved outside the window
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw_Rolling_State(HDC hdc, RECT &paint_area)
{// We draw a rolling out and expanding platform

   switch (Platform_State.Rolling)
   {
   case  EPlatform_Substate_Rolling::Roll_In:
      Draw_Roll_In_State(hdc, paint_area);
      break;

   case EPlatform_Substate_Rolling::Expand_Roll_In:
      Draw_Normal_State(hdc, paint_area); // We draw a platform expanding after rolling out
      break;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Draw_Roll_In_State(HDC hdc, RECT &paint_area)
{// Draw a rolling out platform

   int x = (int)(X_Pos * AsConfig::D_Global_Scale);
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int roller_size = AsConfig::Platform_Circle_Size * AsConfig::Global_Scale;
   double alpha;
   XFORM xform, old_xform;

   // Ball
   Platform_Circle_Color.Select(hdc);

   Ellipse(hdc, x, y, x + roller_size - 1, y + roller_size - 1);

   // Demarcation line
   alpha = -2.0 * M_PI / (double)Max_Rolling_Step * (double)Rolling_Step;

   xform.eM11 = (float)cos(alpha);
   xform.eM12 = (float)sin(alpha);
   xform.eM21 = (float)-sin(alpha);
   xform.eM22 = (float)cos(alpha);
   xform.eDx = (float)(x + roller_size / 2);
   xform.eDy = (float)(y + roller_size / 2);

   GetWorldTransform(hdc, &old_xform);
   SetWorldTransform(hdc, &xform);

   AsConfig::BG_Color.Select(hdc);

   Rectangle(hdc, -AsConfig::Global_Scale / 2, -roller_size / 2, AsConfig::Global_Scale / 2, roller_size / 2);

   SetWorldTransform(hdc, &old_xform);

   // Highlight
   Platform_Expanding.Draw_Circle_Highlight(hdc, x, y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Reflect_On_Circle(double next_x_pos, double next_y_pos, double platform_ball_x_offset, ABall *ball)
{
   double dx, dy;
   double platform_ball_x, platform_ball_y, platform_ball_radius;
   double distance, two_radiuses;
   double beta, alpha, gamma;
   double related_ball_direction;
   const double pi_2 = 2.0 * M_PI;

   platform_ball_radius = (double)AsConfig::Platform_Circle_Size / 2.0;
   platform_ball_x = (double)X_Pos + platform_ball_radius + platform_ball_x_offset;
   platform_ball_y = (double)AsConfig::Platform_Y_Pos + platform_ball_radius;

   dx = next_x_pos - platform_ball_x;
   dy = next_y_pos - platform_ball_y;

   distance = sqrt(pow(dx, 2) + pow(dy, 2));
   two_radiuses = platform_ball_radius + ball->Radius;

   if(distance + AsConfig::Moving_Step_Size < two_radiuses)
   {// The ball touched the side ball on the platform

      beta = atan2(-dy, dx);

      related_ball_direction = ball->Get_Direction();
      related_ball_direction -= beta;

      if (related_ball_direction > pi_2)
         related_ball_direction -= pi_2;

      if (related_ball_direction < 0.0)
         related_ball_direction += pi_2;

      if (related_ball_direction > M_PI_2 && related_ball_direction < M_PI + M_PI_2)
      {
         alpha = beta + M_PI - ball->Get_Direction();
         gamma = beta + alpha;

         ball->Set_Direction(gamma);

         return true;
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Get_Platform_Image_Stroke_Color(int x, int y, const AColor **color, int& stroke_len)
{// Calculates the length of the next vertical stroke

   int offset = y * Normal_Platform_Image_Width + x; // Position in the Normal_Platform_Image array corresponding to the offset (x, y)
   int color_value;

   stroke_len = 0;

   if (y >= Normal_Platform_Image_Height)
      return false;

   for (int i = y; i < Normal_Platform_Image_Height; ++i)
   {
      if (i == y)
      {
         color_value = Normal_Platform_Image[offset];
         stroke_len = 1;
      }
      else
      {
         if (color_value == Normal_Platform_Image[offset])
            ++stroke_len;
         else
            break;
      }

      offset += Normal_Platform_Image_Width; // Go to line below
   }

   if (color_value == Highlight_Color.Get_RGB())
      *color = &Highlight_Color;
   else if (color_value == Platform_Circle_Color.Get_RGB())
      *color = &Platform_Circle_Color;
   else if (color_value == Platform_Inner_Color.Get_RGB())
      *color = &Platform_Inner_Color;
   else if (color_value == AsConfig::BG_Color.Get_RGB())
      *color = &AsConfig::BG_Color;
   else
      AsConfig::Throw();

   return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform::Get_Normal_Platform_Image(HDC hdc)
{
   int x = (int)(X_Pos * AsConfig::D_Global_Scale);
   int y = AsConfig::Platform_Y_Pos * AsConfig::Global_Scale;
   int offset = 0;

   Normal_Platform_Image_Width = AsConfig::Platform_Normal_Width * AsConfig::Global_Scale;
   Normal_Platform_Image_Height = AsConfig::Platform_Height * AsConfig::Global_Scale;

   Normal_Platform_Image = new int[Normal_Platform_Image_Width * Normal_Platform_Image_Height];

   for (int i = 0; i < Normal_Platform_Image_Height; ++i)
      for (int j = 0; j < Normal_Platform_Image_Width; ++j)
         Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double AsPlatform::Get_Current_Width()
{
   if (Platform_State == EPlatform_State::Rolling && Platform_State.Rolling == EPlatform_Substate_Rolling::Roll_In)
      return (double)AsConfig::Platform_Circle_Size;
   else if (Platform_State == EPlatform_State::Expanding)
      return Platform_Expanding.Expanding_Platform_Width;
   else
      return (double)AsConfig::Platform_Normal_Width;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform::Correct_Platform_Pos()
{
   bool got_correction = false;
   double max_platform_x = AsConfig::Max_X_Pos - Get_Current_Width() + 1;

   if (X_Pos <= AsConfig::Border_X_Offset)
   {
      X_Pos = AsConfig::Border_X_Offset;
      got_correction = true;
   }

   if (X_Pos >= max_platform_x)
   {
      X_Pos = max_platform_x;
      got_correction = true;
   }

   return got_correction;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
