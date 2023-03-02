#pragma once

#include "Config.h"

//-----------------------------------------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
   AsBorder();
   void Init();
   void Draw(HDC, RECT &);

private:
   void Draw_Element(HDC, int, int y, bool, HPEN, HBRUSH);

   HPEN Border_Blue_Pen, Border_White_Pen;
   HBRUSH Border_Blue_Brush, Border_White_Brush;
};