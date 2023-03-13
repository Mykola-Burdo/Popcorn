#pragma once

#include "Active_Brick.h"

enum class ELetter_Type
{
   ELT_None = 0,
   ELT_O
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

enum class EFalling_Letter_State
{
   EFLS_Normal,
   EFLS_Finalizing, // Let's start deleting the object
   EFLS_Finished, // When an object can be deleted
};
//-----------------------------------------------------------------------------------------------------------------------------------------------

class AFalling_Letter : public AGraphics_Object
{
public:
   AFalling_Letter(EBrick_Type, ELetter_Type, int, int);

   virtual void Act();
   virtual void Draw(HDC, RECT &);
   virtual bool Is_Finished();

   void Get_Letter_Cell(RECT &);
   void Finalize();

   const EBrick_Type Brick_Type;
   const ELetter_Type Letter_Type;

private:
   void Set_Brick_Letter_Colors(bool, HPEN &, HBRUSH &, HPEN &, HBRUSH &);
   void Draw_Brick_Letter(HDC);

   EFalling_Letter_State Falling_Letter_State;
   int X, Y;
   int Rotation_Step;
   int Next_Rotation_Tick;
   RECT Letter_Cell, Prev_Letter_Cell;

   static const int Ticks_Per_Step = 4;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
