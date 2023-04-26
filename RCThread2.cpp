//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ReadClusterForm.h"
#include "RCThread.h"
#include "RCThread2.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

__fastcall RCThread2::RCThread2(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	int dbopen = sqlite3_open("signatures.db", &DB);
	int result = sqlite3_prepare_v2(DB, "CREATE TABLE IF NOT EXISTS list\
		(id integer primary key autoincrement, numbercluster integer,\
		filetype varchar(255))", -1, &stmt, NULL);
	if (result != SQLITE_OK)
		ShowMessage("Ошибка подготовки запроса на создание таблицы");
	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE)
		ShowMessage("Ошибка создания таблицы");
	int ClearTable = sqlite3_exec(DB,"DELETE FROM list",NULL,NULL,&errmsg);
	int ClearID =sqlite3_exec(DB,"DELETE FROM sqlite_sequence where \
		name='list'", NULL, NULL, &errmsg);
	if (ClearTable != SQLITE_OK || ClearID != SQLITE_OK)
		ShowMessage("Ошибка очистки таблицы");
}
//---------------------------------------------------------------------------
void __fastcall RCThread2::Execute()
{
	FreeOnTerminate = true;
	ID = 0;
	int result = sqlite3_prepare_v2(DB,"INSERT INTO list(numbercluster,\
		filetype) values(?,?)", -1, &stmt, NULL);
	if (result != SQLITE_OK)
		ShowMessage("Ошибка заполенения таблицы") ;
	while (!Form1->RCT1->Finished)
	{
		sqlite3_bind_int(stmt, 1, Form1->RCT1->NumberCluster);
		sqlite3_bind_text(stmt, 2, Form1->RCT1->FileType,
			strlen(Form1->RCT1->FileType), NULL);
		sqlite3_step(stmt);
		sqlite3_reset(stmt);
		ID++;
		Synchronize(&UpdateVST);
		Form1->RCT1->Resume();
		Suspend();
	}
}
//---------------------------------------------------------------------------
void __fastcall RCThread2::UpdateVST()
{
	Form1->VirtualStringTree->BeginUpdate();
	PVirtualNode Node = Form1->VirtualStringTree->AddChild(NULL);
	DataTree* datatree = (DataTree*)Form1->VirtualStringTree->GetNodeData(Node);
	datatree->ID = ID;
	datatree->NumberCluster = Form1->RCT1->NumberCluster;
	datatree->FileType = Form1->RCT1->FileType;
	Form1->VirtualStringTree->EndUpdate();
}
//---------------------------------------------------------------------------
__fastcall RCThread2::~RCThread2()
{
	sqlite3_finalize(stmt);
	sqlite3_close(DB);
}
//---------------------------------------------------------------------------
