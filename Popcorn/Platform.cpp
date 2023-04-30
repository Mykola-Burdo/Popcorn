#include "Platform.h"

//--------------AsPlatform_State--------------------
AsPlatform_State::AsPlatform_State()
   : Current_State(EPlatform_State::Regular), Next_State(EPlatform_State::Unknown), Regular(EPlatform_Substate_Regular::Missing), Meltdown(EPlatform_Substate_Meltdown::Unknown), 
   Rolling(EPlatform_Substate_Rolling::Unknown), Glue(EPlatform_Transformation::Unknown), Expanding(EPlatform_Transformation::Unknown), 
   Laser(EPlatform_Transformation::Unknown), Moving(EPlatform_Moving_State::Stop)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsPlatform_State::operator EPlatform_State() const
{
   return Current_State;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_State::operator = (EPlatform_State new_state)
{
   Current_State = new_state;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_State::Set_Next_State(EPlatform_State next_state)
{
   if(next_state == Current_State)
      return;

   switch (Current_State)
   {
   case EPlatform_State::Regular:
      AsConfig::Throw(); // Ordinary state "self" does not end, switching to another state must be explicit
      break;

   case EPlatform_State::Meltdown:
      return; // We ignore any next state after Meltdown, since after it the game process must be restarted

   case EPlatform_State::Rolling:
      AsConfig::Throw(); // The Rolling state "by itself" should switch to the following
      break;

   case EPlatform_State::Glue:
      Glue = EPlatform_Transformation::Finalize;
      break;

   case EPlatform_State::Expanding:
      Expanding = EPlatform_Transformation::Finalize;
      break;

   case EPlatform_State::Laser:
      Laser = EPlatform_Transformation::Finalize;
      break;

   default:
      AsConfig::Throw();
   }

   Next_State = next_state;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

EPlatform_State AsPlatform_State::Get_Next_State()
{
   return Next_State;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

EPlatform_State AsPlatform_State::Set_State(EPlatform_Substate_Regular new_regular_state)
{
   EPlatform_Transformation *transformation_state = 0;

   if(Current_State == EPlatform_State::Regular && Regular == new_regular_state)
      return EPlatform_State::Unknown;

   if(new_regular_state == EPlatform_Substate_Regular::Normal)
   {
      switch (Current_State)
      {
      case EPlatform_State::Glue:
         transformation_state = &Glue;
         break;

      case EPlatform_State::Expanding:
         transformation_state = &Expanding;
         break;

      case EPlatform_State::Laser:
         transformation_state = &Laser;
         break;
      }

      if(transformation_state != 0)
      {
         if(*transformation_state == EPlatform_Transformation::Unknown)
            return Set_Next_Or_Regular_State(new_regular_state); // State finalization finished
         else
            *transformation_state = EPlatform_Transformation::Finalize; // We start the finalization of the state

         return EPlatform_State::Unknown;
      }
   }

   Current_State = EPlatform_State::Regular;
   Regular = new_regular_state;

   return EPlatform_State::Unknown;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

EPlatform_State AsPlatform_State::Set_Next_Or_Regular_State(EPlatform_Substate_Regular new_regular_state)
{// Return: if not Unknown, then go to this new state

   EPlatform_State next_state;

   Current_State = EPlatform_State::Regular;

   // If there is a pending state, then we will transfer it to it, and not to Regular
   next_state = Get_Next_State();

   if(next_state == EPlatform_State::Unknown)
      Regular = new_regular_state;

   return next_state;
      
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




//--------------AsPlatform_Glue--------------------
const double AsPlatform_Glue::Max_Glue_Spot_Height_Ratio = 1.0;
const double AsPlatform_Glue::Min_Glue_Spot_Height_Ratio = 0.4;
const double AsPlatform_Glue::Glue_Spot_Height_Ratio_Step = 0.05;
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsPlatform_Glue::AsPlatform_Glue(AsPlatform_State &platform_state)
   : Glue_Spot_Height_Ratio(0.0), Platform_State(&platform_state)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform_Glue::Act(AsBall_Set *ball_set, EPlatform_State &next_state)
{
   next_state = EPlatform_State::Unknown;

   switch (Platform_State->Glue)
   {
   case EPlatform_Transformation::Init:
      if(Glue_Spot_Height_Ratio < Max_Glue_Spot_Height_Ratio)
         Glue_Spot_Height_Ratio += Glue_Spot_Height_Ratio_Step;
      else
         Platform_State->Glue = EPlatform_Transformation::Active;

      return true;

   case EPlatform_Transformation::Active:
      break;

   case EPlatform_Transformation::Finalize:
      if(Glue_Spot_Height_Ratio > Min_Glue_Spot_Height_Ratio)
      {
         Glue_Spot_Height_Ratio -= Glue_Spot_Height_Ratio_Step;

         while(ball_set->Release_Next_Ball())
         {
         }
      }
      else
      {
         Platform_State->Glue = EPlatform_Transformation::Unknown;
         next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
      }

      return true;

   default:
      AsConfig::Throw();
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Glue::Draw_State(HDC hdc, double x_pos)
{// Draw a platform with spreading glue

   HRGN region;
   RECT glue_rect;

   glue_rect.left = (int)((x_pos + 5.0) * AsConfig::D_Global_Scale);
   glue_rect.top = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;
   glue_rect.right = glue_rect.left + AsPlatform::Normal_Platform_Inner_Width * AsConfig::Global_Scale;
   glue_rect.bottom = glue_rect.top + (AsPlatform::Height - 2) * AsConfig::Global_Scale;

   region = CreateRectRgnIndirect(&glue_rect);
   SelectClipRgn(hdc, region);

   AsConfig::BG_Color.Select(hdc);
   Draw_Glue_Spot(hdc, x_pos, 0, 9, 5);
   Draw_Glue_Spot(hdc, x_pos, 6, 6, 5);
   Draw_Glue_Spot(hdc, x_pos, 9, 9, 6);

   AsConfig::White_Color.Select(hdc);
   Draw_Glue_Spot(hdc, x_pos, 0, 9, 4);
   Draw_Glue_Spot(hdc, x_pos, 6, 6, 4);
   Draw_Glue_Spot(hdc, x_pos, 9, 9, 5);

   SelectClipRgn(hdc, 0);
   DeleteObject(region);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Glue::Reset()
{
   Glue_Spot_Height_Ratio = Min_Glue_Spot_Height_Ratio;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Glue::Draw_Glue_Spot(HDC hdc, double x_pos, int x_offset, int width, int height)
{// Draw a spot of glue

   RECT spot_rect;
   int platform_top = (AsConfig::Platform_Y_Pos + 1) * AsConfig::Global_Scale;
   int spot_height = (int)((double)height * AsConfig::D_Global_Scale * Glue_Spot_Height_Ratio);

   // Draw a semi-ellipse as a "spot" of glue

   spot_rect.left = (int)((x_pos + 5.0 + (double)x_offset) * AsConfig::D_Global_Scale);
   spot_rect.top = platform_top - spot_height;
   spot_rect.right = spot_rect.left + width * AsConfig::Global_Scale;
   spot_rect.bottom = platform_top + spot_height - AsConfig::Global_Scale;

   Chord(hdc, spot_rect.left, spot_rect.top, spot_rect.right - 1, spot_rect.bottom - 1, spot_rect.left, platform_top - 1, spot_rect.right - 1, platform_top - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




//--------------AsPlatform_Expanding--------------------
const double AsPlatform_Expanding::Max_Expanding_Platform_Width = 40.0;
const double AsPlatform_Expanding::Min_Expanding_Platform_Width = (double)AsPlatform::Normal_Width;
const double AsPlatform_Expanding::Expanding_Platform_Width_Step = 1.0;
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsPlatform_Expanding::~AsPlatform_Expanding()
{
   delete Truss_Color;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsPlatform_Expanding::AsPlatform_Expanding(AsPlatform_State &platform_state)
   : Expanding_Platform_Width(0.0), Platform_State(&platform_state), Highlight_Color(0), Circle_Color(0), Inner_Color(0), Truss_Color(0)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Expanding::Init(AColor &highlight_color, AColor &circle_color, AColor &inner_color)
{
   Highlight_Color = &highlight_color;
   Circle_Color = &circle_color;
   Inner_Color = &inner_color;
   Truss_Color = new AColor(inner_color, AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform_Expanding::Act(double &x_pos, EPlatform_State &next_state, bool &correct_pos)
{
   next_state = EPlatform_State::Unknown;
   correct_pos = false;

   switch (Platform_State->Expanding)
   {
   case EPlatform_Transformation::Init:
      if(Expanding_Platform_Width < Max_Expanding_Platform_Width)
      {
         Expanding_Platform_Width += Expanding_Platform_Width_Step;
         x_pos -= Expanding_Platform_Width_Step / 2.0;
         correct_pos = true;
      }
      else
         Platform_State->Expanding = EPlatform_Transformation::Active;

      return true;

   case EPlatform_Transformation::Active:
      break;

   case EPlatform_Transformation::Finalize:
      if(Expanding_Platform_Width > Min_Expanding_Platform_Width)
      {
         Expanding_Platform_Width -= Expanding_Platform_Width_Step;
         x_pos += Expanding_Platform_Width_Step / 2.0;
         correct_pos = true;
      }
      else
      {
         Platform_State->Expanding = EPlatform_Transformation::Unknown;
         next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
      }

      return true;

   default:
      AsConfig::Throw();
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Expanding::Draw_State(HDC hdc, double x)
{// Drawing an expanding platform

   int y = AsConfig::Platform_Y_Pos;
   const int scale = AsConfig::Global_Scale;
   const double d_scale = AsConfig::D_Global_Scale;
   RECT inner_rect;

   inner_rect.left = (int)((x + (Expanding_Platform_Width - (double)AsPlatform::Expanding_Platform_Inner_Width) / 2.0) * d_scale);
   inner_rect.top = (y + 1) * scale;
   inner_rect.right = inner_rect.left + AsPlatform::Expanding_Platform_Inner_Width * scale;
   inner_rect.bottom = (y + 1 + 5) * scale;

   // Left-hand side
   // Ball
   Draw_Expanding_Platform_Ball(hdc, x, true);

   // Trusses
   Draw_Expanding_Truss(hdc, inner_rect, true);

   // Right side
   // Ball
   Draw_Expanding_Platform_Ball(hdc, x, false);

   // Trusses
   Draw_Expanding_Truss(hdc, inner_rect, false);

   // Draw the middle part
   Inner_Color->Select(hdc);

   Rectangle(hdc, inner_rect.left, inner_rect.top, inner_rect.right - 1, inner_rect.bottom - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Expanding::Draw_Circle_Highlight(HDC hdc, int x, int y)
{// Draw a highlight on the ball

   const int scale = AsConfig::Global_Scale;
   int size = (AsPlatform::Circle_Size - 1) * scale - 1;

   Highlight_Color->Select_Pen(hdc);

   Arc(hdc, x + scale, y + scale, x + size, y + size, x + 2 * scale, y + scale, x + scale, y + 4 * scale);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Expanding::Reset()
{
   Expanding_Platform_Width = Min_Expanding_Platform_Width;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Expanding::Draw_Expanding_Platform_Ball(HDC hdc, double x, bool is_left)
{// Draw the side ball of the expanding platform

   int y = AsConfig::Platform_Y_Pos;
   int arc_mid_x;
   int arc_start_y, arc_end_y, arc_right_offset;
   const int scale = AsConfig::Global_Scale;
   const double d_scale = AsConfig::D_Global_Scale;
   RECT rect, arc_rect;

   // Ball
   if(is_left)
      rect.left = (int)(x * d_scale);
   else
      rect.left = (int)((x + Expanding_Platform_Width - (double)AsPlatform::Circle_Size) * d_scale);

   rect.top = y * scale;
   rect.right = rect.left + AsPlatform::Circle_Size * scale;
   rect.bottom = (y + AsPlatform::Circle_Size) * scale;

   Circle_Color->Select(hdc);
   Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

   // Truss adapter
   if(is_left)
      Rectangle(hdc, rect.left + 4 * scale, rect.top, rect.right - scale + 1, rect.bottom - 1);
   else
      Rectangle(hdc, rect.left + 1, rect.top, rect.left + 3 * scale, rect.bottom - 1);

   // Draw a highlight
   Draw_Circle_Highlight(hdc, (int)(x * d_scale), y * scale);

   // Arc truss on the ball
   arc_rect.left = rect.left + 4 * scale + 2;
   arc_rect.top = rect.top + scale + 1;
   arc_rect.right = rect.left + (4 + 3) * scale + 2;
   arc_rect.bottom = rect.bottom - scale - 1;

   arc_mid_x = arc_rect.left + (arc_rect.right - arc_rect.left) / 2;

   if(is_left)
   {
      arc_start_y = arc_rect.top;
      arc_end_y = arc_rect.bottom;
   }
   else
   {
      arc_start_y = arc_rect.bottom;
      arc_end_y = arc_rect.top;

      arc_right_offset = (AsPlatform::Circle_Size - 2) * scale + 1;

      arc_rect.left -= arc_right_offset;
      arc_rect.right -= arc_right_offset;
      arc_mid_x -= arc_right_offset;
   }

   // A hole in the ball under the arc
   AsConfig::BG_Color.Select(hdc);
   Ellipse(hdc, arc_rect.left, arc_rect.top, arc_rect.right - 1, arc_rect.bottom - 1);

   // The arc itself
   Truss_Color->Select(hdc);
   Arc(hdc, arc_rect.left, arc_rect.top, arc_rect.right - 1, arc_rect.bottom - 1, arc_mid_x, arc_start_y, arc_mid_x, arc_end_y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Expanding::Draw_Expanding_Truss(HDC hdc, RECT &inner_rect, bool is_left)
{// Draw trusses of an expanding platform

   const int scale = AsConfig::Global_Scale;
   int truss_x;
   int truss_top_y, truss_low_y;
   int truss_x_offset;
   double extension_ratio; // [1.0 ... 0.0]

   extension_ratio = (Max_Expanding_Platform_Width - Expanding_Platform_Width) / (Max_Expanding_Platform_Width - Min_Expanding_Platform_Width);
   truss_x_offset = (int)(6.0 * extension_ratio * AsConfig::D_Global_Scale);

   truss_x = inner_rect.left + 1;

    if(is_left)
       truss_x += truss_x_offset;
    else
    {
      truss_x += (AsPlatform::Expanding_Platform_Inner_Width + 8 - 1) * scale + 1;
      truss_x -= truss_x_offset;
    }

   truss_top_y = inner_rect.top + 1;
   truss_low_y = inner_rect.bottom - scale + 1;

   Truss_Color->Select(hdc);
   MoveToEx(hdc, truss_x, truss_top_y, 0);
   LineTo(hdc, truss_x - 4 * scale - 1, truss_low_y);
   LineTo(hdc, truss_x - 8 * scale, truss_top_y);

   MoveToEx(hdc, truss_x, truss_low_y, 0);
   LineTo(hdc, truss_x - 4 * scale - 1, truss_top_y);
   LineTo(hdc, truss_x - 8 * scale, truss_low_y);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------



//--------------ALaser_Beam--------------------

ALaser_Beam::ALaser_Beam()
   : Is_Active(false), X_Pos(0.0), Y_Pos(0.0), Beam_Rect{}
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Begin_Movement()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Finish_Movement()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Advance(double)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double ALaser_Beam::Get_Speed()
{

   return 0.0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Act()
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Clear(HDC hdc, RECT &paint_area)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Draw(HDC hdc, RECT &paint_area)
{
   int x_pos, y_pos;
   RECT intersection_rect;

   if(!IntersectRect(&intersection_rect, &paint_area, &Beam_Rect))
      return;

   AsConfig::Laser_Color.Select(hdc);

   x_pos = (int)(X_Pos * AsConfig::D_Global_Scale);
   y_pos = (int)(Y_Pos * AsConfig::D_Global_Scale);

   MoveToEx(hdc, x_pos, y_pos + 1, 0);
   LineTo(hdc, x_pos, y_pos + Height * AsConfig::Global_Scale - 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool ALaser_Beam::Is_Finished()
{

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void ALaser_Beam::Set_At(double x_pos, double y_pos)
{
   X_Pos = x_pos;
   Y_Pos = y_pos;

   Beam_Rect.left = (int)((X_Pos - (double)Width / 2.0) * AsConfig::D_Global_Scale);
   Beam_Rect.top = (int)(Y_Pos * AsConfig::D_Global_Scale);
   Beam_Rect.right = Beam_Rect.left + Width * AsConfig::Global_Scale;
   Beam_Rect.bottom = Beam_Rect.top + Height * AsConfig::Global_Scale;

   AsConfig::Invalidate_Rect(Beam_Rect);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------



//--------------AsLaser_Beam_Set--------------------

void AsLaser_Beam_Set::Begin_Movement()
{
   for (int i = 0; i < Max_Laser_Beam_Count; ++i)
      Laser_Beams[i].Begin_Movement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLaser_Beam_Set::Finish_Movement()
{
   for (int i = 0; i < Max_Laser_Beam_Count; ++i)
      Laser_Beams[i].Finish_Movement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLaser_Beam_Set::Advance(double max_speed)
{
   for (int i = 0; i < Max_Laser_Beam_Count; ++i)
      Laser_Beams[i].Advance(max_speed);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double AsLaser_Beam_Set::Get_Speed()
{
   double curr_speed, max_speed = 0.0;

   for (int i = 0; i < Max_Laser_Beam_Count; ++i)
   {
      curr_speed = Laser_Beams[i].Get_Speed();

      if (curr_speed > max_speed)
         max_speed = curr_speed;
   }

   return max_speed;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLaser_Beam_Set::Act()
{
   // Stub. Not used, because the rays themselves do nothing (not animated)
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLaser_Beam_Set::Clear(HDC hdc, RECT &paint_area)
{
   for (int i = 0; i < Max_Laser_Beam_Count; ++i)
      Laser_Beams[i].Clear(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLaser_Beam_Set::Draw(HDC hdc, RECT &paint_area)
{
   for (int i = 0; i < Max_Laser_Beam_Count; ++i)
      Laser_Beams[i].Draw(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLaser_Beam_Set::Is_Finished()
{
   return false; // Stub because this method is not used
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLaser_Beam_Set::Fire(bool fire_on, double x_pos)
{
   ALaser_Beam *left_beam = 0, *right_beam = 0;

   for(int i = 0; i < Max_Laser_Beam_Count; ++i)
   {
      if(Laser_Beams[i].Is_Active)
         continue;

      if(left_beam == 0)
         left_beam = &Laser_Beams[i];
      else
         if(right_beam == 0)
         {
            right_beam = &Laser_Beams[i];
            break;
         }
   }

   if(left_beam == 0 || right_beam == 0)
      AsConfig::Throw(); // Not enough free laser beams

   left_beam->Set_At(x_pos + 3.0, AsConfig::Platform_Y_Pos);
   right_beam->Set_At(x_pos + (AsPlatform::Normal_Width - 4), AsConfig::Platform_Y_Pos);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




//--------------AsPlatform_Laser--------------------
AsPlatform_Laser::~AsPlatform_Laser()
{
   delete Gun_Color;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsPlatform_Laser::AsPlatform_Laser(AsPlatform_State &platform_state)
   : Laser_Transformation_Step(0), Platform_State(&platform_state), Laser_Beam_Set(0), Circle_Color(0), Inner_Color(0), Gun_Color(0)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Init(AsLaser_Beam_Set *laser_beam_set, AColor &highlight_color, AColor &circle_color, AColor &inner_color)
{
   Laser_Beam_Set = laser_beam_set;
   Circle_Color = &circle_color;
   Inner_Color = &inner_color;

   Gun_Color = new AColor(highlight_color, AsConfig::Global_Scale);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsPlatform_Laser::Act(EPlatform_State &next_state)
{
   next_state = EPlatform_State::Unknown;

   switch (Platform_State->Laser)
   {
   case EPlatform_Transformation::Init:
      if(Laser_Transformation_Step < Max_Laser_Transformation_Step)
         ++Laser_Transformation_Step;
      else
         Platform_State->Laser = EPlatform_Transformation::Active;

      return true;

   case EPlatform_Transformation::Active:
      break;

   case EPlatform_Transformation::Finalize:
      if(Laser_Transformation_Step > 0)
         --Laser_Transformation_Step;
      else
      {
         Platform_State->Laser = EPlatform_Transformation::Unknown;
         next_state = Platform_State->Set_State(EPlatform_Substate_Regular::Normal);
      }

      return true;

   default:
      AsConfig::Throw();
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Draw_State(HDC hdc, double x_pos, RECT &platform_rect)
{// Drawing a laser platform

   HRGN region;

   region = CreateRectRgnIndirect(&platform_rect);
   SelectClipRgn(hdc, region);

   // Left wing
   Draw_Laser_Wing(hdc, x_pos, true);

   // Right wing
   Draw_Laser_Wing(hdc, x_pos, false);

   // Central part
   Draw_Laser_Inner_Part(hdc, x_pos);

   // Left leg
   Draw_Laser_Leg(hdc, x_pos, true);

   // Right leg
   Draw_Laser_Leg(hdc, x_pos, false);

   // Cabin
   Draw_Laser_Cabin(hdc, x_pos);

   SelectClipRgn(hdc, 0);
   DeleteObject(region);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Reset()
{
   Laser_Transformation_Step = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Fire(bool fire_on, double x_pos)
{
   if(Platform_State->Laser != EPlatform_Transformation::Active)
      return; // Ignore the shot until the platform is formed

   if(!fire_on)
      return;

   Laser_Beam_Set->Fire(fire_on, x_pos);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Draw_Laser_Wing(HDC hdc, double x_pos, bool is_left)
{// Draw the wing of the laser platform

   double x, y;
   int x_offset;
   double height;
   int half_max_step = Max_Laser_Transformation_Step / 2;
   double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;
   const int scale = AsConfig::Global_Scale;
   const double d_scale = AsConfig::D_Global_Scale;

   y = AsConfig::Platform_Y_Pos;
   x = x_pos;

   if(!is_left)
      x += AsPlatform::Normal_Width - AsPlatform::Circle_Size;

   // Wing itself
   Circle_Color->Select(hdc);

   Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x, y, 7, 7, ratio, x, y + 1, 7, 12);

   // Jumper
   if(is_left)
      x_offset = 5;
   else
      x_offset = -4;
      
   Draw_Expanding_Figure(hdc, EFigure_Type::Rectangle, x + 3, y + 6, 1, 1, ratio, x + x_offset, y + 2, 6, 5);

   // A gun
   if(Laser_Transformation_Step>= half_max_step)
   {
      ratio = (double)(Laser_Transformation_Step - half_max_step) / (double)half_max_step;

      Gun_Color->Select(hdc);

      if(is_left)
         x = (x_pos + 3.0);
      else
         x = x_pos + (AsPlatform::Normal_Width - 4);

      height = 3.0 * (1.0 - ratio) * d_scale;

      MoveToEx(hdc, (int)(x * d_scale + 1.0), (int)(y * d_scale + 3.0 * d_scale + 1.0), 0);
      LineTo(hdc, (int)(x * d_scale + 1.0), (int)(y * d_scale + height + 1.0));

      // Tail
      Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 1, y + 5, 0, 0, ratio, x - 1, y + 5 + 1.0 / d_scale, 3, 6);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Draw_Laser_Inner_Part(HDC hdc, double x)
{// Draw a shrinking part of a regular platform

   double y;
   double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;

   y = AsConfig::Platform_Y_Pos;

   Inner_Color->Select(hdc);
   Draw_Expanding_Figure(hdc, EFigure_Type::Round_Rect_3x, x + 4, y + 1, 20, 5, ratio, x + 10, y + 3, 8, 1);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Draw_Laser_Leg(HDC hdc, double x_pos, bool is_left)
{// Draw the "leg" of the laser platform

   double x, y;
   double x_scale;
   double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;
   const double d_scale = AsConfig::D_Global_Scale;

   Inner_Color->Select(hdc);

   if(is_left)
   {
      x = (x_pos + 6.0) * d_scale;
      x_scale = d_scale;
   }
   else
   {
       x = x_pos * d_scale + (AsPlatform::Normal_Width - 6) * d_scale - 1.0;
       x_scale = -d_scale;
   }
   y = (double)(AsConfig::Platform_Y_Pos + 3) * d_scale;
   
   POINT left_leg_points[7] =
   {
      {(int)x, (int)y}, {(int)(x + 2.0 * x_scale), (int)(y - 2.0 * d_scale)}, {(int)(x + 4 * x_scale), (int)(y - 2 * d_scale)}, 
      {(int)(x + 4 * x_scale), (int)y}, {(int)(x + 2 * x_scale), (int)(y + 2 * d_scale)}, 
      {(int)(x + 2 * x_scale), (int)(y + (2.0 + 2.0 * ratio) * d_scale)}, {(int)x, (int)(y + 4.0 * ratio * d_scale)}
   };
   Polygon(hdc, left_leg_points, 7);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Draw_Laser_Cabin(HDC hdc, double x)
{// Draw the cabin of the laser platform

   double y;
   double ratio = (double)Laser_Transformation_Step / (double)Max_Laser_Transformation_Step;
   double one_pixel = 1.0 / AsConfig::D_Global_Scale;
   const int scale = AsConfig::Global_Scale;

   y = AsConfig::Platform_Y_Pos;

   // Outer part
   Inner_Color->Select_Pen(hdc);
   Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 13, y + 1, 2, 1, ratio, x + 9, y - 1, 10, 8 - one_pixel);

   // Middle ring
   AsConfig::BG_Color.Select(hdc);
   Draw_Expanding_Figure(hdc, EFigure_Type::Rectangle, x + 13, y + 1, 2, 1, ratio, x + 11, y, 6, 1);
   Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 13, y + 1, 2, 1, ratio, x + 10, y, 8, 5 - one_pixel);

   // Inner part
   AsConfig::White_Color.Select(hdc);
   Draw_Expanding_Figure(hdc, EFigure_Type::Ellipse, x + 13, y + 1, 2, 1, ratio, x + 11, y, 6, 4 - one_pixel);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsPlatform_Laser::Draw_Expanding_Figure(HDC hdc, EFigure_Type figure_type, double start_x, double start_y, double start_width, double start_height, double ratio, double end_x, double end_y, double end_width, double end_height)
{
   int x, y;
   int width, height;
   RECT rect;

   x = Get_Expanding_Value(start_x, end_x, ratio);
   y = Get_Expanding_Value(start_y, end_y, ratio);
   width = Get_Expanding_Value(start_width, end_width, ratio);
   height = Get_Expanding_Value(start_height, end_height, ratio);

   switch (figure_type)
   {
   case EFigure_Type::Ellipse:
      Ellipse(hdc, x, y, x + width - 1, y + height);
      break;

   case EFigure_Type::Rectangle:
      Rectangle(hdc, x, y, x + width - 1, y + height);
      break;

   case EFigure_Type::Round_Rect_3x:
      rect.left = x;
      rect.top = y;
      rect.right = x + width;
      rect.bottom = y + height + 1;

      AsConfig::Round_Rect(hdc, rect, 3);
      break;

   default:
      AsConfig::Throw();
   }     
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsPlatform_Laser::Get_Expanding_Value(double start, double end, double ratio)
{
   int value;
   double delta;

   delta = end - start;
   value = (int)((start + delta * ratio) * AsConfig::D_Global_Scale);

   return value;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------




//--------------AsPlatform--------------------
AsPlatform::~AsPlatform()
{
   delete[] Normal_Platform_Image;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsPlatform::AsPlatform()
   : X_Pos(AsConfig::Border_X_Offset), Normal_Platform_Image_Width(0), Normal_Platform_Image_Height(0), Normal_Platform_Image(0), 
   Inner_Width(Normal_Platform_Inner_Width), Rolling_Step(0), Last_Redraw_timer_Tick(0), Speed(0.0), Ball_Set(0), Platform_Glue(Platform_State), 
   Platform_Expanding(Platform_State), Platform_Laser(Platform_State), Left_Key_Down(false), Right_Key_Down(false), Platform_Rect{}, 
   Prev_Platform_Rect{}, Highlight_Color(255, 255, 255), Platform_Circle_Color(151, 0, 0), Platform_Inner_Color(0, 128, 192)
{
   X_Pos = (AsConfig::Max_X_Pos - Normal_Width) / 2;
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
   inner_low_y = (double)(AsConfig::Platform_Y_Pos + Height - 1);
   inner_left_x = (double)(X_Pos + Circle_Size - 1);
   inner_right_x = (double)(X_Pos + Get_Current_Width() - (Circle_Size - 1));

   // Checking the reflection from the side balls
   if(Reflect_On_Circle(next_x_pos, next_y_pos, 0.0, ball))
      goto _on_hit; // From left

   if (Reflect_On_Circle(next_x_pos, next_y_pos, Get_Current_Width() - Circle_Size, ball))
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
      if(Platform_Laser.Act(next_state))
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
   Platform_Rect.bottom = Platform_Rect.top + Height * AsConfig::Global_Scale;


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
   if (!key_down)
      return;

   if(Has_State(EPlatform_Substate_Regular::Ready))
   {
      Ball_Set->Release_From_Platform(Get_Middle_Pos());
      Set_State(EPlatform_Substate_Regular::Normal);
   }
   else
      if(Platform_State == EPlatform_State::Glue)
         Ball_Set->Release_Next_Ball();
      else if(Platform_State == EPlatform_State::Laser)
         Platform_Laser.Fire(key_down, X_Pos);
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
      if(Inner_Width >= Normal_Platform_Inner_Width)
      {
         Inner_Width = Normal_Platform_Inner_Width;
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
   rect.right = (int)((x + (double)Circle_Size) * d_scale);
   rect.bottom = (y + Circle_Size) * scale;
   Ellipse(hdc, rect.left, rect.top, rect.right - 1, rect.bottom - 1);

   rect.left = (int)((x + Inner_Width) * d_scale);
   rect.top = y * scale;
   rect.right = (int)((x + (double)Circle_Size + Inner_Width) * d_scale);
   rect.bottom = (y + Circle_Size) * scale;
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
   int roller_size = Circle_Size * AsConfig::Global_Scale;
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

   platform_ball_radius = (double)Circle_Size / 2.0;
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

   Normal_Platform_Image_Width = Normal_Width * AsConfig::Global_Scale;
   Normal_Platform_Image_Height = Height * AsConfig::Global_Scale;

   Normal_Platform_Image = new int[Normal_Platform_Image_Width * Normal_Platform_Image_Height];

   for (int i = 0; i < Normal_Platform_Image_Height; ++i)
      for (int j = 0; j < Normal_Platform_Image_Width; ++j)
         Normal_Platform_Image[offset++] = GetPixel(hdc, x + j, y + i);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double AsPlatform::Get_Current_Width()
{
   if (Platform_State == EPlatform_State::Rolling && Platform_State.Rolling == EPlatform_Substate_Rolling::Roll_In)
      return (double)Circle_Size;
   else if (Platform_State == EPlatform_State::Expanding)
      return Platform_Expanding.Expanding_Platform_Width;
   else
      return (double)Normal_Width;
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
