//---------------------------------------------------------------------------

#ifndef RCThread2H
#define RCThread2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class RCThread2 : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall RCThread2(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
