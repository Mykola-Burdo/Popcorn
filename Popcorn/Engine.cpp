#include "Engine.h"



//-----------------------------------------------------------------------------------------------------------------------------------------------
AsEngine::AsEngine()
   : Hwnd(0), BG_Pen(0), BG_Brush(0)
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Init(HWND hwnd)
{// Game setup at startup

   Hwnd = hwnd;

   AsConfig::Create_Pen_Brush(0, 0, 0, BG_Pen, BG_Brush);

   Ball.Init();
   Level.Init();
   Platform.Init();
   Border.Init();
   
   Platform.Redraw_Platform(Hwnd);

   SetTimer(Hwnd, Timer_ID, 1000 / AsConfig::FPS, 0);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

void AsEngine::Draw_Frame(HDC hdc, RECT &paint_area)
{// Drawing the game screen

   Level.Draw(hdc, paint_area);

   Platform.Draw(hdc, BG_Pen, BG_Brush, paint_area);

   /*
   for (int i = 0; i < 16; ++i)
   {
      Draw_Brick_Letter(hdc, 650, (AsConfig::Level_Y_Offset + 36) + i * AsConfig::Cell_Height * AsConfig::Global_Scale, i, EBrick_Type::EBT_Red, ELetter_Type::ELT_O);
   }
   */

   Ball.Draw(hdc, paint_area, BG_Pen, BG_Brush);

   Border.Draw(hdc, paint_area, BG_Pen, BG_Brush);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsEngine::On_Key_Down(EKey_Type key_type)
{
   switch (key_type)
   {
   case EKey_Type::EKT_Left:
      Platform.X_Pos -= Platform.X_Step;

      if (Platform.X_Pos <= AsConfig::Border_X_Offset)
         Platform.X_Pos = AsConfig::Border_X_Offset;

      Platform.Redraw_Platform(Hwnd);
      break;

   case EKey_Type::EKT_Right:
      Platform.X_Pos += Platform.X_Step;

      if (Platform.X_Pos >= AsConfig::Max_X_Pos - Platform.Width + 1)
         Platform.X_Pos = AsConfig::Max_X_Pos - Platform.Width + 1;

      Platform.Redraw_Platform(Hwnd);
      break;

   case EKey_Type::EKT_Space:
      break;
   }

   return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------

int AsEngine::On_Timer()
{
   Ball.Move(Hwnd, &Level, Platform.X_Pos, Platform.Width);

   Level.Active_Brick.Act(Hwnd);

   return 0;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
