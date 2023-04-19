//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "RCThread2.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall RCThread2::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall RCThread2::RCThread2(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void __fastcall RCThread2::Execute()
{
	//---- Place thread code here ----
}
//---------------------------------------------------------------------------
