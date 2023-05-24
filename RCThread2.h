//---------------------------------------------------------------------------

#ifndef RCThread2H
#define RCThread2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "sqlite3.h"
//---------------------------------------------------------------------------
class RCThread2 : public TThread
{
private:
	sqlite3 *DB;
	sqlite3_stmt *stmt;
	const char *ErrorMessage;
	char *errmsg;
	int ID;
	ULONG NCluster;
	const char *FType;
	void __fastcall ShowErrMsg();
	void __fastcall UpdateVST();
protected:
	void __fastcall Execute();
public:
	__fastcall RCThread2(bool CreateSuspended);
    __fastcall ~RCThread2();
};
//---------------------------------------------------------------------------
#endif
