//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ReadClusterForm.h"
#include "RCThread.h"
#include "RCThread2.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
bool __fastcall RCThread::isjpg(BYTE Cluster[])
{
	for (int i = 0; i < sizeof(jpg); i++)
	{
		if (Cluster[i] != jpg[i])
			return false;
	}
	return true;
}

bool __fastcall RCThread::ispng(BYTE Cluster[])
{
	for (int i = 0; i < sizeof(png); i++)
	{
		if (Cluster[i] != png[i])
			return false;
	}
	return true;
}

bool __fastcall RCThread::isbmp(BYTE Cluster[])
{
	for (int i = 0; i < sizeof(bmp); i++)
	{
		if (Cluster[i] != bmp[i])
			return false;
	}
	return true;
}
//---------------------------------------------------------------------------
__fastcall RCThread::RCThread(bool CreateSuspended, std::wstring path)
	: TThread(CreateSuspended)
{
	std::wstring Path = L"\\\\.\\" + path + L":";
	DiskOpen = CreateFileW(Path.c_str(), GENERIC_READ, FILE_SHARE_READ |
		FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (DiskOpen == INVALID_HANDLE_VALUE)
		Terminate();
	Timer = CreateWaitableTimer(NULL, false, NULL);
	DWORD SectorPerCluster, BytesPerSector, NumOfFrClusters;
	std::wstring PathFCS = path + L":\\";
	GetDiskFreeSpaceW(PathFCS.c_str(), &SectorPerCluster, &BytesPerSector,
		&NumOfFrClusters, &TotNumOfClusters);
	ClusterSize = SectorPerCluster * BytesPerSector;
}
//---------------------------------------------------------------------------
void __fastcall RCThread::Execute()
{
   	FreeOnTerminate = true;
	LARGE_INTEGER TimeResponse = { 0 };
	LONG Period = 500;
	SetWaitableTimer(Timer, &TimeResponse, Period, NULL, NULL, false);
	DWORD ReadedBytes;
	int BlockM = 32;
	int BlockSize = ClusterSize * BlockM;
	Cluster = new BYTE[BlockSize];
	BYTE Sign[10];
	for (NumberCluster = 0; NumberCluster < TotNumOfClusters;)
	{
		bool ReadResult = ReadFile(DiskOpen, Cluster, BlockSize,
			&ReadedBytes, NULL);
		if (!ReadResult || ReadedBytes != BlockSize)
			Terminate();
		if (WaitForSingleObject(Timer, 0) == WAIT_OBJECT_0)
			Synchronize(&UpdatePB);
		for (int i = 0; i < BlockSize; i += ClusterSize)
		{
			Signature = NULL;
			for (int ii = 0; ii < 10; ii++)
			{
				Sign[ii] = Cluster[i+ii];
			}
			if (Form1->jpg->Checked)
			{
				if (isjpg(Sign))
					Signature = ".jpg/.jpeg";
			}
			if (Form1->png->Checked)
			{
				if (ispng(Sign))
					Signature = ".png";
			}
			if (Form1->bmp->Checked)
			{
				if (isbmp(Sign))
					Signature = ".bmp";
			}
			if (Signature != NULL)
			{
				Form1->RCT2->Resume();
				Suspend();
			}
			NumberCluster++;
		}
	}
	Synchronize(&EndOfThread);
}
//---------------------------------------------------------------------------
void __fastcall RCThread::UpdatePB()
{
	Form1->ProgressBar->Position = int(NumberCluster * 100 / TotNumOfClusters);
}
//---------------------------------------------------------------------------
void __fastcall RCThread::EndOfThread()
{
	Form1->InfoLabel->Caption = "Готово!";
	Form1->ProgressBar->Position = 100;
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
	delete[] Cluster;
}
//---------------------------------------------------------------------------
