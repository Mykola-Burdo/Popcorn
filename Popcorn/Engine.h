#pragma once

#include <Windows.h>
#include "Border.h"
#include "Ball.h"
#include "Platform.h"

enum class EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};
const int Timer_ID = WM_USER + 1;



//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsEngine
{
public:
   AsEngine();
   void Init(HWND);
   void Draw_Frame(HDC, RECT &);
   int On_Key_Down(EKey_Type);
   int On_Timer();

private:
   HWND Hwnd;
   HPEN BG_Pen;
   HBRUSH BG_Brush;

   ABall Ball;
   ALevel Level;
   AsPlatform Platform;
   AsBorder Border;
};
