#pragma once

#include "Active_Brick.h"

enum class ELetter_Type
{
   ELT_None = 0,
   ELT_O
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AFalling_Letter : public AGraphics_Object
{
public:
   AFalling_Letter(EBrick_Type, ELetter_Type, int, int);

   virtual void Act();
   virtual void Draw(HDC, RECT&);
   virtual bool Is_Finished();

   const EBrick_Type Brick_Type;
   const ELetter_Type Letter_Type;

   bool Got_Hit;

private:
   void Set_Brick_Letter_Colors(bool, HPEN&, HBRUSH&, HPEN&, HBRUSH&);
   void Draw_Brick_Letter(HDC);

   int X, Y;
   int Rotation_Step;
   int Next_Rotation_Tick;
   RECT Letter_Cell, Prev_Letter_Cell;

   static const int Ticks_Per_Step = 4;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
