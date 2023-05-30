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
	int dbopen = sqlite3_open("signs.db", &DB);
	int result = sqlite3_prepare_v2(DB, "CREATE TABLE IF NOT EXISTS list\
		(id integer primary key autoincrement, numbercluster integer,\
		filetype varchar(255))", -1, &stmt, NULL);
	if (result != SQLITE_OK)
	{
		ErrorMessage = "Ошибка подготовки запроса CREATE TABLE";
		Synchronize(&ShowErrMsg);
		Terminate();
	}
	result = sqlite3_step(stmt);
	if (result != SQLITE_DONE)
	{
		ErrorMessage = "Ошибка создания таблицы";
		Synchronize(&ShowErrMsg);
		Terminate();
	}
	int ClearTable = sqlite3_exec(DB,"DELETE FROM list", NULL, NULL, &errmsg);
	int ClearID = sqlite3_exec(DB,"DELETE FROM sqlite_sequence where \
		name='list'", NULL, NULL, &errmsg);
	if (ClearTable != SQLITE_OK || ClearID != SQLITE_OK)
	{
		ErrorMessage = "Ошибка очистки таблицы";
		Synchronize(&ShowErrMsg);
		Terminate();
	}
}
//---------------------------------------------------------------------------
void __fastcall RCThread2::Execute()
{
	FreeOnTerminate = true;
	ID = 0;
	int result = sqlite3_prepare_v2(DB,"INSERT INTO list(numbercluster,\
		filetype) values(?,?)", -1, &stmt, NULL);
	if (result != SQLITE_OK)
	{
		ErrorMessage = "Ошибка подготовки запроса INSERT";
		Synchronize(&ShowErrMsg);
		Terminate();
	}
	pair <ULONG, const char *> entry;
	while (!Form1->RCT1->Finished || !TSQ.empty())
	{
		if (Form1->NeedStop)
			Free();
		entry = TSQ.frontpop();
		if (entry.first != NULL && entry.second != NULL)
		{
			NCluster = entry.first;
			FType = entry.second;
			ID++;
			sqlite3_bind_int(stmt, 1, NCluster);
			sqlite3_bind_text(stmt, 2, FType, strlen(FType), NULL);
			sqlite3_step(stmt);
			sqlite3_reset(stmt);
			Synchronize(&UpdateVST);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall RCThread2::ShowErrMsg()
{
    Form1->NeedStop = true;
	Form1->ProgressBar->Visible = false;
	Form1->InfoLabel->Caption = ErrorMessage;
	Form1->ChooseVolume->Enabled = true;
	Form1->StopButton->Enabled = false;
	Form1->FindButton->Enabled = true;
	Form1->jpg->Enabled = true;
	Form1->png->Enabled = true;
	Form1->bmp->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall RCThread2::UpdateVST()
{
	Form1->VirtualStringTree->BeginUpdate();
	PVirtualNode Node = Form1->VirtualStringTree->AddChild(NULL);
	DataTree* datatree = (DataTree*)Form1->VirtualStringTree->GetNodeData(Node);
	datatree->ID = ID;
	datatree->NumberCluster = NCluster;
	datatree->FileType = FType;
	Form1->VirtualStringTree->EndUpdate();
}
//---------------------------------------------------------------------------
__fastcall RCThread2::~RCThread2()
{
	sqlite3_finalize(stmt);
	sqlite3_close(DB);
}
//---------------------------------------------------------------------------
