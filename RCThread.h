//---------------------------------------------------------------------------

#ifndef RCThreadH
#define RCThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class RCThread : public TThread
{
private:
protected:
	void __fastcall Execute();
public:
	__fastcall RCThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
