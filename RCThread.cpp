//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ReadClusterForm.h"
#include "RCThread.h"
#include "RCThread2.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall RCThread::RCThread(bool CreateSuspended, wstring path)
	: TThread(CreateSuspended)
{
	wstring Path = L"\\\\.\\" + path + L":";
	DiskOpen = CreateFileW(Path.c_str(), GENERIC_READ, FILE_SHARE_READ |
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (DiskOpen == INVALID_HANDLE_VALUE)
	{
		ErrorMessage = "Ошибка открытия диска";
		Synchronize(&ShowErrMsg);
		Terminate();
	}
	BYTE BootRec[512];
   	DWORD ReadedBytes;
	bool ReadResult = ReadFile(DiskOpen, BootRec, sizeof(BootRec),
		&ReadedBytes, NULL);
	GetFileSystem *FS = new GetFileSystem(BootRec);
	if (FS->ErrorMessage != NULL)
	{
		ErrorMessage = FS->ErrorMessage;
		Synchronize(&ShowErrMsg);
		Terminate();
	}
	ClusterSize = FS->ClusterSize;
	TotNumOfClusters = FS->TotNumOfClusters;
	StartOffset = FS->StartOffset;
	delete FS;
	Timer = CreateWaitableTimer(NULL, false, NULL);
}
//---------------------------------------------------------------------------
void __fastcall RCThread::Execute()
{
	FreeOnTerminate = true;
	chrono::time_point start = chrono::system_clock::now();
	if (StartOffset.QuadPart != 0)
		NumberCluster = 2;
	else
		NumberCluster = 0;
	LARGE_INTEGER TimeResponse = { 0 };
	LONG Period = 500;
	SetWaitableTimer(Timer, &TimeResponse, Period, NULL, NULL, false);
	DWORD ReadedBytes;
	int BlockM = 131072 / ClusterSize;
	int BlockSize = ClusterSize * BlockM;
	Block = new BYTE[BlockSize];
	BYTE Signature[10];
	pair <ULONG, const char *> entry;
	SetFilePointer(DiskOpen, StartOffset.LowPart,
		&StartOffset.HighPart, FILE_BEGIN);
	for (NumberCluster; NumberCluster < TotNumOfClusters;
		NumberCluster += BlockM)
	{
		if (Form1->NeedStop)
			Free();
		bool ReadResult = ReadFile(DiskOpen, Block, BlockSize,
			&ReadedBytes, NULL);
		if (!ReadResult)
		{
			ErrorMessage = "Ошибка чтения диска";
			Synchronize(&ShowErrMsg);
			Terminate();
		}
		if (!ReadResult || ReadedBytes != BlockSize)
			Terminate();
		if (WaitForSingleObject(Timer, 0) == WAIT_OBJECT_0)
			Synchronize(&UpdatePB);
		BlockIterator *IBID =
			new ImageBlockIteratorDecorator(*Block, BlockSize, ClusterSize);
		for (IBID->First(); !IBID->IsDone(); IBID->Next())
		{
			FileType = NULL;
			BYTE *Current = IBID->GetCurrent();
			int Add = Current[0];
			int Type = Current[1];
			if (Type == 1 && Form1->jpg->Checked)
				FileType = ".jpg/.jpeg";
			if (Type == 2 && Form1->png->Checked)
				FileType = ".png";
			if (Type == 3 && Form1->bmp->Checked)
				FileType = ".bmp";
			entry = {NumberCluster + Add, FileType};
			Form1->RCT2->TSQ.push(entry);
		}
		delete IBID;
	}
	chrono::time_point end = chrono::system_clock::now();
	chrono::duration<double> sec = end - start;
	ResSec = "Готово! Затраченное время: " + to_string(sec.count()) + " сек";
	Synchronize(&EndOfThread);
}
//---------------------------------------------------------------------------
void __fastcall RCThread::ShowErrMsg()
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
void __fastcall RCThread::UpdatePB()
{
	Form1->ProgressBar->Position = (double)NumberCluster / TotNumOfClusters * 100;
}
//---------------------------------------------------------------------------
void __fastcall RCThread::EndOfThread()
{
	Form1->ProgressBar->Position = 100;
	Form1->InfoLabel->Caption = ResSec.c_str();
    Form1->ChooseVolume->Enabled = true;
	Form1->StopButton->Enabled = false;
	Form1->FindButton->Enabled = true;
	Form1->jpg->Enabled = true;
	Form1->png->Enabled = true;
	Form1->bmp->Enabled = true;
}
//---------------------------------------------------------------------------
__fastcall RCThread::~RCThread()
{
	CloseHandle(DiskOpen);
	CloseHandle(Timer);
	delete[] Block;
}
//---------------------------------------------------------------------------
