//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ReadClusterForm.h"
#include "RCThread.h"
#include "RCThread2.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool __fastcall RCThread::isjpg(BYTE Signature[])
{
	for (int i = 0; i < sizeof(jpg); i++)
	{
		if (Signature[i] != jpg[i])
			return false;
	}
	return true;
}

bool __fastcall RCThread::ispng(BYTE Signature[])
{
	for (int i = 0; i < sizeof(png); i++)
	{
		if (Signature[i] != png[i])
			return false;
	}
	return true;
}

bool __fastcall RCThread::isbmp(BYTE Signature[])
{
	for (int i = 0; i < sizeof(bmp); i++)
	{
		if (Signature[i] != bmp[i])
			return false;
	}
	return true;
}
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
		Free();
	}
	Timer = CreateWaitableTimer(NULL, false, NULL);
	DWORD SectorPerCluster, BytesPerSector, NumOfFrClusters;
	wstring PathFCS = path + L":\\";
	SecThread = CreateEventA(NULL, TRUE, FALSE, NULL);
	GetDiskFreeSpaceW(PathFCS.c_str(), &SectorPerCluster, &BytesPerSector,
		&NumOfFrClusters, &TotNumOfClusters);
	ClusterSize = SectorPerCluster * BytesPerSector;
}
//---------------------------------------------------------------------------
void __fastcall RCThread::Execute()
{
   	FreeOnTerminate = true;
    chrono::time_point start = chrono::system_clock::now();
    int EvSt;
	LARGE_INTEGER TimeResponse = { 0 };
	LONG Period = 500;
	SetWaitableTimer(Timer, &TimeResponse, Period, NULL, NULL, false);
	DWORD ReadedBytes;
	int BlockM = 32;
	int BlockSize = ClusterSize * BlockM;
	Cluster = new BYTE[BlockSize];
	BYTE Signature[10];
	for (NumberCluster = 0; NumberCluster < TotNumOfClusters;)
	{
		bool ReadResult = ReadFile(DiskOpen, Cluster, BlockSize,
			&ReadedBytes, NULL);
		if (!ReadResult)
		{
            ErrorMessage = "Ошибка чтения диска";
			Synchronize(&ShowErrMsg);
			Free();
		}
		if (!ReadResult || ReadedBytes != BlockSize)
			Terminate();
		if (WaitForSingleObject(Timer, 0) == WAIT_OBJECT_0)
			Synchronize(&UpdatePB);
		for (int i = 0; i < BlockSize; i += ClusterSize)
		{
			if (Form1->NeedStop)
				Free();
			FileType = NULL;
			for (int ii = 0; ii < 10; ii++)
			{
				Signature[ii] = Cluster[i + ii];
			}
			if (Form1->jpg->Checked)
			{
				if (isjpg(Signature))
					FileType = ".jpg/.jpeg";
			}
			if (Form1->png->Checked)
			{
				if (ispng(Signature))
					FileType = ".png";
			}
			if (Form1->bmp->Checked)
			{
				if (isbmp(Signature))
					FileType = ".bmp";
			}
			EvSt = WaitForSingleObject(SecThread, 0);
			if (FileType != NULL && EvSt == 0)
				Suspend();
			EvSt = WaitForSingleObject(SecThread, 0);
			if (FileType != NULL && EvSt != 0)
			{
				NumClus = NumberCluster;
				FiType = FileType;
				SetEvent(SecThread);
			}
			NumberCluster++;
		}
	}
	chrono::time_point end = chrono::system_clock::now();
	chrono::duration<double> sec = end - start;
	ressec = "Готово! Затраченное время: " + to_string(sec.count()) + " сек";
	Synchronize(&EndOfThread);
}
//---------------------------------------------------------------------------
void __fastcall RCThread::ShowErrMsg()
{
	Form1->ProgressBar->Enabled = false;
	Form1->InfoLabel->Caption = ErrorMessage;
	Form1->StopButton->Enabled = false;
	Form1->FindButton->Enabled = true;
	Form1->jpg->Enabled = true;
	Form1->png->Enabled = true;
	Form1->bmp->Enabled = true;	
}
//---------------------------------------------------------------------------
void __fastcall RCThread::UpdatePB()
{
	Form1->ProgressBar->Position = int(NumberCluster * 100 / TotNumOfClusters);
}
//---------------------------------------------------------------------------
void __fastcall RCThread::EndOfThread()
{
	Form1->ProgressBar->Position = 100;
	Form1->InfoLabel->Caption = ressec.c_str();
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
	CloseHandle(SecThread);
	delete[] Cluster;
}
//---------------------------------------------------------------------------
