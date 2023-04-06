#include "Ball_Set.h"

//--------------AsBall_Set--------------------
void AsBall_Set::Begin_Movement()
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Begin_Movement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Finish_Movement()
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Finish_Movement();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Advance(double max_speed)
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Advance(max_speed);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

double AsBall_Set::Get_Speed()
{
   double curr_speed, max_speed = 0.0;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_speed = Balls[i].Get_Speed();

      if (curr_speed > max_speed)
         max_speed = curr_speed;
   }

   return max_speed;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Act()
{
   // Stub because this method is not used
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Clear(HDC hdc, RECT& paint_area)
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Clear(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Draw(HDC hdc, RECT& paint_area)
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Draw(hdc, paint_area);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsBall_Set::Is_Finished()
{
   return false; // Stub because this method is not used
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Release_From_Platform(double platform_x_pos)
{
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
      if (Balls[i].Get_State() == EBall_State::EBS_On_Platform)
         Balls[i].Set_State(EBall_State::EBS_Normal, platform_x_pos, AsConfig::Start_Ball_Y_Pos);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Release_Next_Ball()
{
   double ball_x, ball_y;
   ABall *curr_ball;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::EBS_On_Platform)
      {
         curr_ball->Get_Center(ball_x, ball_y);
         curr_ball->Set_State(EBall_State::EBS_Normal, ball_x, ball_y);
         break;
      }
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Set_On_Platform(double platform_x_pos)
{
   //for (i = 0; i < 3; ++i)
   Balls[0].Set_State(EBall_State::EBS_On_Platform, platform_x_pos, AsConfig::Start_Ball_Y_Pos);

   for (int i = 1; i < AsConfig::Max_Balls_Count; ++i)
      Balls[i].Set_State(EBall_State::EBS_Disabled);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsBall_Set::All_Balls_Are_Lost()
{
   int active_balls_count = 0;
   int lost_balls_count = 0;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      if (Balls[i].Get_State() == EBall_State::EBS_Disabled)
         continue;

      ++active_balls_count;

      if (Balls[i].Get_State() == EBall_State::EBS_Lost)
      {
         ++lost_balls_count;
         continue;
      }
   }

   if (active_balls_count == lost_balls_count)
      return true;
   else
      return false;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Set_For_Test()
{
   Balls[0].Set_For_Test(); // Only the ball with index 0 participates in repeated tests
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

bool AsBall_Set::Is_Test_Finished()
{
   return Balls[0].Is_Test_Finished();  // Only the ball with index 0 participates in repeated tests
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Triple_Balls()
{// The farthest ball flying from the platform turns into three balls

   ABall* curr_ball;
   ABall* further_ball = 0;
   ABall* left_candidate = 0, * right_candidate = 0;
   double curr_ball_x, curr_ball_y;
   double further_ball_x, further_ball_y;

   // Choose the farthest ball in Y
   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() != EBall_State::EBS_Normal)
         continue;

      if (further_ball == 0)
         further_ball = curr_ball;
      else
      {
         curr_ball->Get_Center(curr_ball_x, curr_ball_y);
         further_ball->Get_Center(further_ball_x, further_ball_y);

         if (curr_ball_y < further_ball_y)
            further_ball = curr_ball;
      }
   }

   // If there is a "normal" ball, then multiply it
   if (further_ball == 0)
      return;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_ball = &Balls[i];

      switch (curr_ball->Get_State())
      {
      case EBall_State::EBS_Disabled:
      case EBall_State::EBS_Lost:
         if (left_candidate == 0)
            left_candidate = curr_ball;
         else
            if (right_candidate == 0)
            {
               right_candidate = curr_ball;
               break; // Both candidates found
            }
      }
   }

   // We part the side balls to the sides
   if (left_candidate != 0)
   {
      *left_candidate = *further_ball;
      left_candidate->Set_Direction(left_candidate->Get_Direction() + M_PI / 8.0);
   }

   if (right_candidate != 0)
   {
      *right_candidate = *further_ball;
      right_candidate->Set_Direction(right_candidate->Get_Direction() - M_PI / 8.0);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Inverse_Ball()
{// Reverse direction for all balls

   ABall* curr_ball;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::EBS_Normal)
         curr_ball->Set_Direction(curr_ball->Get_Direction() + M_PI);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Accelerate()
{
   ABall* curr_ball;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::EBS_Normal)
         curr_ball->Set_Speed(curr_ball->Get_Speed() * AsConfig::Ball_Acceleration);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsBall_Set::Reset_Speed()
{
   ABall* curr_ball;

   for (int i = 0; i < AsConfig::Max_Balls_Count; ++i)
   {
      curr_ball = &Balls[i];

      if (curr_ball->Get_State() == EBall_State::EBS_Normal)
         curr_ball->Set_Speed(AsConfig::Normal_Ball_Speed);
   }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
