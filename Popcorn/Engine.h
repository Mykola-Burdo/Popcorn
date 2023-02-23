#pragma once

#include <Windows.h>

enum class EKey_Type
{
   EKT_Left,
   EKT_Right,
   EKT_Space
};

const int Global_Scale = 3;
const int Timer_ID = WM_USER + 1;

void Init_Engine(HWND);
void Draw_Frame(HDC, RECT &);
int On_Key_Down(EKey_Type);
int On_Timer();