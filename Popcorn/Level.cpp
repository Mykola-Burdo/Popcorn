#include "Level.h"

char AsLevel::Level_01[AsConfig::Level_Height][AsConfig::Level_Width] =
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

char AsLevel::Test_Level[AsConfig::Level_Height][AsConfig::Level_Width] =
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

//--------------AsLevel--------------------
AsLevel::~AsLevel()
{
   Cancel_All_Activity();

   delete[] Teleport_Bricks_Pos;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AsLevel::AsLevel()
   : Level_Rect{}, Need_To_Cancel_All(false), Active_Bricks_Count(0), Falling_Letters_Count(0), Teleport_Bricks_Count(0), Teleport_Bricks_Pos(0),
   Parachute_Color(AsConfig::Red_Color, AsConfig::Blue_Color, AsConfig::Global_Scale), Advertisement(0)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLevel::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
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
   if(min_level_x < 0)
      min_level_x = 0;

   max_level_x = (int)((max_ball_x - AsConfig::Level_X_Offset) / (double)AsConfig::Cell_Width);
   if (max_level_x >= AsConfig::Level_Width - 1)
      max_level_x = AsConfig::Level_Width - 1;

   min_level_y = (int)((min_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);
   if (min_level_y < 0)
      min_level_y = 0;

   max_level_y = (int)((max_ball_y - AsConfig::Level_Y_Offset) / (double)AsConfig::Cell_Height);
   if (max_level_y >= AsConfig::Level_Height - 1)
      max_level_y = AsConfig::Level_Height - 1;

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
            if(On_Hit(j, i, ball, true))
            {
               if(vertical_reflection_pos < horizontal_reflection_pos)
                  ball->Reflect(true);
               else
                  ball->Reflect(false);
            }

            return true;
         }
         else
            if(got_horizontal_hit)
            {
               if(On_Hit(j, i, ball, false))
                  ball->Reflect(false);
    
               return true;
            }
            else
               if(got_vertical_hit)
               {
                  if(On_Hit(j, i, ball, true))
                     ball->Reflect(true);
                  
                  return true;
               }
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Act()
{
   Act_Objects((AGraphics_Object**)&Active_Bricks, Active_Bricks_Count, AsConfig::Max_Active_Bricks_Count);

   Act_Objects((AGraphics_Object**)&Falling_Letters, Falling_Letters_Count, AsConfig::Max_Falling_Letters_Count);

   if (Advertisement != 0)
      Advertisement->Act();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Clear(HDC hdc, RECT &paint_area)
{// Erase moving objects

   Clear_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);

   if (Advertisement != 0)
      Advertisement->Clear(hdc, paint_area);

   if (Need_To_Cancel_All)
   {
      Cancel_All_Activity();
      Need_To_Cancel_All = false;
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Draw(HDC hdc, RECT &paint_area)
{// Output all objects of the level

   RECT intersection_rect, brick_rect;

   if (Advertisement != 0)
      Advertisement->Draw(hdc, paint_area);

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
               Draw_Brick(hdc, brick_rect, j, i);
         }

      Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Active_Bricks, AsConfig::Max_Active_Bricks_Count);
   }

   Draw_Objects(hdc, paint_area, (AGraphics_Object**)&Falling_Letters, AsConfig::Max_Falling_Letters_Count);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLevel::Is_Finished()
{
   return false; // Stub because this method is not used
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Init()
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

void AsLevel::Set_Current_Level(char level[AsConfig::Level_Height][AsConfig::Level_Width])
{
   int index;
   EBrick_Type brick_type;

   memcpy(Current_Level, level, sizeof(Current_Level));

   // Counting teleports
   Teleport_Bricks_Count = 0;

   for(int i = 0; i < AsConfig::Level_Height; ++i)
   {
      for (int j = 0; j < AsConfig::Level_Width; ++j)
      {
         brick_type = (EBrick_Type)Current_Level[i][j];

         if(brick_type == EBrick_Type::EBT_Teleport)
            ++Teleport_Bricks_Count;
      }
   }

   delete[] Teleport_Bricks_Pos;
   Teleport_Bricks_Pos = 0;

   // Saving teleport coordinates
   if(Teleport_Bricks_Count != 0)
   {
      if(Teleport_Bricks_Count == 1)
         AsConfig::Throw(); // Teleports must be more than 1!

      Teleport_Bricks_Pos = new SPoint[Teleport_Bricks_Count];
      index = 0;

      for (int i = 0; i < AsConfig::Level_Height; ++i)
      {
         for (int j = 0; j < AsConfig::Level_Width; ++j)
         {
            brick_type = (EBrick_Type)Current_Level[i][j];

            if (brick_type == EBrick_Type::EBT_Teleport)
            {
               Teleport_Bricks_Pos[index].X = j;
               Teleport_Bricks_Pos[index].Y = i;
	            ++index;
            }
         }
      }
   }

   Advertisement = new AAdvertisement(9, 6, 2, 3);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLevel::Get_Next_Falling_Letter(int &index, AFalling_Letter **falling_letter)
{
   AFalling_Letter *current_letter;

   if(Falling_Letters_Count == 0)
      return false;

   if(index < 0 || index >= AsConfig::Max_Falling_Letters_Count)
      return false;

   while(index < AsConfig::Max_Falling_Letters_Count)
   {
      current_letter = Falling_Letters[index++];

      if(current_letter != 0)
      {
         *falling_letter = current_letter;
         return true;
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Stop()
{
   Need_To_Cancel_All = true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLevel::On_Hit(int brick_x, int brick_y, ABall *ball, bool vertical_hit)
{
   EBrick_Type brick_type;
   bool can_reflect = true;

   brick_type = (EBrick_Type)Current_Level[brick_y][brick_x];

   if(brick_type == EBrick_Type::EBT_Parachute)
   {
      ball->Set_On_Parachute(brick_x, brick_y);
      Current_Level[brick_y][brick_x] = (char)EBrick_Type::EBT_None;
   }
   else if(Add_Falling_Letter(brick_x, brick_y, brick_type))
      Current_Level[brick_y][brick_x] = (char)EBrick_Type::EBT_None;
   else
      can_reflect = Create_Active_Brick(brick_x, brick_y, brick_type, ball, vertical_hit);

   Redraw_Brick(brick_x, brick_y);

   return can_reflect;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Redraw_Brick(int brick_x, int brick_y)
{
   RECT brick_rect;

   brick_rect.left = (AsConfig::Level_X_Offset + brick_x * AsConfig::Cell_Width) * AsConfig::Global_Scale;
   brick_rect.top = (AsConfig::Level_Y_Offset + brick_y * AsConfig::Cell_Height) * AsConfig::Global_Scale;
   brick_rect.right = brick_rect.left + AsConfig::Brick_Width * AsConfig::Global_Scale;
   brick_rect.bottom = brick_rect.top + AsConfig::Brick_Height * AsConfig::Global_Scale;

   InvalidateRect(AsConfig::Hwnd, &brick_rect, FALSE);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLevel::Add_Falling_Letter(int brick_x, int brick_y, EBrick_Type brick_type)
{// Create a falling letter if we can

   int letter_x, letter_y;
   ELetter_Type letter_type;

   AFalling_Letter *falling_letter;

   if (!(brick_type == EBrick_Type::EBT_Red || brick_type == EBrick_Type::EBT_Blue))
      return false;

   if (AsConfig::Rand(AsConfig::Hits_Per_Letter) != 0)
      return false;

   if (Falling_Letters_Count >= AsConfig::Max_Falling_Letters_Count)
      return false;
   
   for (int i = 0; i < AsConfig::Max_Falling_Letters_Count; ++i)
   {
      if (Falling_Letters[i] == 0)
      {
         letter_x = (brick_x * AsConfig::Cell_Width + AsConfig::Level_X_Offset) * AsConfig::Global_Scale;
         letter_y = (brick_y * AsConfig::Cell_Height + AsConfig::Level_Y_Offset) * AsConfig::Global_Scale;

         //letter_type = AFalling_Letter::Get_Random_Letter_Type();
         letter_type = ELetter_Type::ELT_K;

         falling_letter = new AFalling_Letter(brick_type, letter_type, letter_x, letter_y);
         Falling_Letters[i] = falling_letter;
         ++Falling_Letters_Count;
         return true;
      }
   }

   return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLevel::Create_Active_Brick(int brick_x, int brick_y, EBrick_Type brick_type, ABall *ball, bool vertical_hit)
{// Create an active brick if we can

   //int another_brick_x, another_brick_y;

   AActive_Brick *active_brick = 0;

   if (Active_Bricks_Count >= AsConfig::Max_Active_Bricks_Count)
      return true; // There are too many active bricks!

   switch (brick_type)
   {
   case EBrick_Type::EBT_None:
      return true;

   case EBrick_Type::EBT_Red:
   case EBrick_Type::EBT_Blue:
      active_brick = new AActive_Brick_Red_Blue(brick_type, brick_x, brick_y);
      Current_Level[brick_y][brick_x] = (char)EBrick_Type::EBT_None;
      break;

   case EBrick_Type::EBT_Unbreakable:
      active_brick = new AActive_Brick_Unbreakable(brick_x, brick_y);
      break;

   case EBrick_Type::EBT_Multihit_1:
      active_brick = new AActive_Brick_Multihit(brick_x, brick_y);
      Current_Level[brick_y][brick_x] = (char)EBrick_Type::EBT_None;
      break;

   case EBrick_Type::EBT_Multihit_2:
   case EBrick_Type::EBT_Multihit_3:
   case EBrick_Type::EBT_Multihit_4:
      Current_Level[brick_y][brick_x] = (char)brick_type - 1;
      break;


   case EBrick_Type::EBT_Parachute:
      AsConfig::Throw(); // An active brick is not created for a parachute!!!
      break;

   case EBrick_Type::EBT_Teleport:
      Add_Active_Brick_Teleport(brick_x, brick_y, ball, vertical_hit);
      return false;

   case EBrick_Type::EBT_Ad:
      active_brick = new AActive_Brick_Ad(brick_x, brick_y, Advertisement);
      Current_Level[brick_y][brick_x] = (char)EBrick_Type::EBT_Invisible;
      break;

   case EBrick_Type::EBT_Invisible:
      return true;

   default:
      AsConfig::Throw();

   }

   if(active_brick != 0)
      Add_New_Active_Brick(active_brick);

   return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Add_Active_Brick_Teleport(int brick_x, int brick_y, ABall *ball, bool vertical_hit)
{
   bool got_direction;
   EDirection_Type direction;
   int dest_brick_x, dest_brick_y;
   double pre_teleport_x_pos, pre_teleport_y_pos;
   double curr_ball_x_pos, curr_ball_y_pos;

   AActive_Brick_Teleport *source_teleport, *destination_teleport;

   ball->Get_Center(pre_teleport_x_pos, pre_teleport_y_pos); // The position of the ball in front of the entrance to the teleport

   destination_teleport = Select_Destination_Teleport(brick_x, brick_y);
   source_teleport = new AActive_Brick_Teleport(brick_x, brick_y, ball, destination_teleport);

   destination_teleport->Get_Level_Pos(dest_brick_x, dest_brick_y);

   // After creating the teleport, the ball became in its center
   ball->Get_Center(curr_ball_x_pos, curr_ball_y_pos);

   // Finding the direction the ball came from
   if(vertical_hit)
   {
      if(pre_teleport_y_pos < curr_ball_y_pos)
         direction = EDirection_Type::EDT_Down;
      else
         direction = EDirection_Type::EDT_Up;
   }
   else
   {
      if (pre_teleport_x_pos < curr_ball_x_pos)
         direction = EDirection_Type::EDT_Right;
      else
         direction = EDirection_Type::EDT_Left;
   }

   // Going through all directions in search of free
   got_direction = false;

   for(int i = 0; i < 4; ++i)
   {
      switch (direction)
      {
      case EDirection_Type::EDT_Left:
         if (dest_brick_x > 0 && Current_Level[dest_brick_y][dest_brick_x - 1] == (int)EBrick_Type::EBT_None)
            got_direction = true;
         break;

      case EDirection_Type::EDT_Up:
         if (dest_brick_y > 0 && Current_Level[dest_brick_y - 1][dest_brick_x] == (int)EBrick_Type::EBT_None)
            got_direction = true;
         break;

      case EDirection_Type::EDT_Right:
         if (dest_brick_x < AsConfig::Level_Width - 1 && Current_Level[dest_brick_y][dest_brick_x + 1] == (int)EBrick_Type::EBT_None)
            got_direction = true;
         break;

      case EDirection_Type::EDT_Down:
         if (dest_brick_y < AsConfig::Level_Height - 1 && Current_Level[dest_brick_y + 1][dest_brick_x] == (int)EBrick_Type::EBT_None)
            got_direction = true;
         break;

      default:
         AsConfig::Throw();
      }

      if(got_direction)
         break;

      direction = (EDirection_Type)((int)direction - 1);
      ball->Set_Direction(ball->Get_Direction() + M_PI_2);

      if((int)direction < 0)
         direction = EDirection_Type::EDT_Down;
   }

   destination_teleport->Release_Direction = direction;

   Add_New_Active_Brick(source_teleport);
   Add_New_Active_Brick(destination_teleport);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Add_New_Active_Brick(AActive_Brick *active_brick)
{
   // Adding a new active brick to the first free space
   for (int i = 0; i < AsConfig::Max_Active_Bricks_Count; ++i)
   {
      if (Active_Bricks[i] == 0)
      {
         Active_Bricks[i] = active_brick;
         ++Active_Bricks_Count;
         break;
      }
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

AActive_Brick_Teleport *AsLevel::Select_Destination_Teleport(int source_x, int source_y)
{
   int dest_index;

   AActive_Brick_Teleport *destination_teleport;

   if(Teleport_Bricks_Count < 2)
   {
      AsConfig::Throw();
      return 0;
   }

   dest_index = AsConfig::Rand(Teleport_Bricks_Count);

   if(Teleport_Bricks_Pos[dest_index].X == source_x && Teleport_Bricks_Pos[dest_index].Y == source_y)
   {// If you accidentally chose the current teleport - go to the next

      ++dest_index;

      if(dest_index >= Teleport_Bricks_Count)
         dest_index = 0; // Go to the beginning of the array, if you have gone beyond it
   }


   destination_teleport = new AActive_Brick_Teleport(Teleport_Bricks_Pos[dest_index].X, Teleport_Bricks_Pos[dest_index].Y, 0, 0);

   return destination_teleport;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsLevel::Check_Vertical_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall *ball, double &reflection_pos)
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

bool AsLevel::Check_Horizontal_Hit(double next_x_pos, double next_y_pos, int level_x, int level_y, ABall* ball, double &reflection_pos)
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

void AsLevel::Draw_Brick(HDC hdc, RECT &brick_rect, int level_x, int level_y)
{// The conclusion of the "brick"

   const AColor *color = 0;
   EBrick_Type brick_type = (EBrick_Type)Current_Level[level_y][level_x];

   switch (brick_type)
   {
   case EBrick_Type::EBT_None:
      if(Advertisement != 0 && Advertisement->Has_Brick_At(level_x, level_y))
         break;
      // else - No break!

   case EBrick_Type::EBT_Red:
   case EBrick_Type::EBT_Blue:
      AActive_Brick_Red_Blue::Draw_In_Level(hdc, brick_rect, brick_type);
      break;

   case EBrick_Type::EBT_Unbreakable:
      AActive_Brick_Unbreakable::Draw_In_Level(hdc, brick_rect);
      break;

   case EBrick_Type::EBT_Multihit_1:
   case EBrick_Type::EBT_Multihit_2:
   case EBrick_Type::EBT_Multihit_3:
   case EBrick_Type::EBT_Multihit_4:
      AActive_Brick_Multihit::Draw_In_Level(hdc, brick_rect, brick_type);
      break;

   case EBrick_Type::EBT_Parachute:
      Draw_Parachute_In_Level(hdc, brick_rect);
      break;

   case EBrick_Type::EBT_Teleport:
      AActive_Brick_Teleport::Draw_In_Level(hdc, brick_rect);
      break;

   case EBrick_Type::EBT_Ad:
      AActive_Brick_Ad::Draw_In_Level(hdc, brick_rect);
      break;

   case EBrick_Type::EBT_Invisible:
      break;

   default:
      AsConfig::Throw();
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Draw_Parachute_In_Level(HDC hdc, RECT &brick_rect)
{
   Draw_Parachute_Part(hdc, brick_rect, 0, 4);
   Draw_Parachute_Part(hdc, brick_rect, 4, 6);
   Draw_Parachute_Part(hdc, brick_rect, 10, 4);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Draw_Parachute_Part(HDC hdc, RECT& brick_rect, int offset, int width)
{
   const int scale = AsConfig::Global_Scale;
   RECT rect;

   // Upper segment
   rect.left = brick_rect.left + offset * scale + 1;
   rect.top = brick_rect.top + 1;
   rect.right = rect.left + width * scale + 1;
   rect.bottom = rect.top + 3 * scale + 1;

   Parachute_Color.Select(hdc);
   AsConfig::Round_Rect(hdc, rect);

   // lower segment
   rect.top += 3 * scale;
   rect.bottom += 3 * scale;

   AsConfig::Round_Rect(hdc, rect);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Clear_Objects(HDC hdc, RECT &paint_area, AGraphics_Object **objects_array, int objects_max_count)
{
   for (int i = 0; i < objects_max_count; ++i)
   {
      if (objects_array[i] != 0)
         objects_array[i]->Clear(hdc, paint_area);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Draw_Objects(HDC hdc, RECT  &paint_area, AGraphics_Object** objects_array, int objects_max_count)
{
   for (int i = 0; i < objects_max_count; ++i)
   {
      if (objects_array[i] != 0)
         objects_array[i]->Draw(hdc, paint_area);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Act_Objects(AGraphics_Object **objects_array, int &objects_count, const int objects_max_count)
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
            --objects_count;
         }
      }
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Delete_Objects(AGraphics_Object **objects_array, int &objects_count, int objects_max_count)
{
   for (int i = 0; i < objects_max_count; ++i)
   {
      if (objects_array[i] != 0)
      {
         delete objects_array[i];
         objects_array[i] = 0;
      }
   }

   objects_count = 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsLevel::Cancel_All_Activity()
{
   Delete_Objects((AGraphics_Object**)&Active_Bricks, Active_Bricks_Count, AsConfig::Max_Active_Bricks_Count);
   Delete_Objects((AGraphics_Object**)&Falling_Letters, Falling_Letters_Count, AsConfig::Max_Falling_Letters_Count);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
