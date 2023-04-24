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
protected:
	sqlite3 *DB;
	sqlite3_stmt *stmt;
	char *errmsg;
    int ID;
	void __fastcall Execute();
public:
	__fastcall RCThread2(bool CreateSuspended);
    void __fastcall UpdateVST();
    __fastcall ~RCThread2();
};
//---------------------------------------------------------------------------
#endif
