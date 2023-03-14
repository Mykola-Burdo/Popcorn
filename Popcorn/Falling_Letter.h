#pragma once

#include "Active_Brick.h"

enum class ELetter_Type
{
   ELT_None,
   ELT_O, // "Cancel"
   ELT_I, // "Inversion"
   ELT_C, // "Acceleration"
   ELT_M, // "Monsters"
   ELT_G, // "Life"
   ELT_K, // "Glue"
   ELT_W, // "Extension"
   ELT_P, // "Floor"
   ELT_L, // "Laser"
   ELT_T, // "Three"
   ELT_Plus // "Moving to the next level"
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
   void Test_Draw_All_Steps(HDC);

   const EBrick_Type Brick_Type;
   const ELetter_Type Letter_Type;

private:
   void Set_Brick_Letter_Colors(bool, HPEN &, HBRUSH &, HPEN &, HBRUSH &);
   void Draw_Brick_Letter(HDC);
   void Draw_Line(HDC, int, int, int, int);

   EFalling_Letter_State Falling_Letter_State;
   int X, Y;
   int Rotation_Step;
   int Next_Rotation_Tick;
   RECT Letter_Cell, Prev_Letter_Cell;

   static const int Ticks_Per_Step = 4;
   static const int Max_Rotation_Step = 16;
   static const int Brick_Half_Height = AsConfig::Brick_Height * AsConfig::Global_Scale / 2;
};
//-----------------------------------------------------------------------------------------------------------------------------------------------
