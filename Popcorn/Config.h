#pragma once

#include <Windows.h>
#define _USE_MATH_DEFINES
#include <cmath>

class AsConfig
{
public:
	static void Create_Pen_Brush(unsigned char, unsigned char, unsigned char, HPEN &, HBRUSH &);

	static const int Global_Scale = 3;
   static const int Level_Width = 12; // Level width in cells
   static const int Level_Height = 14; // Level height in cells
   static const int Cell_Width = 16;
   static const int Cell_Height = 8;
   static const int Level_X_Offset = 8;
   static const int Level_Y_Offset = 6;
   static const int Border_X_Offset = 6;
   static const int Border_Y_Offset = 4;
   static const int Ball_Size = 4;
   static const int Max_X_Pos = Level_X_Offset + Cell_Width * Level_Width;
   static const int Max_Y_Pos = 199 - Ball_Size;
   static const int Y_Pos = 185;
};
