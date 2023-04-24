//---------------------------------------------------------------------------

#ifndef RCThreadH
#define RCThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "RCThread2.h"
//---------------------------------------------------------------------------
class RCThread : public TThread
{
private:
	bool __fastcall isjpg(BYTE*);
	bool __fastcall ispng(BYTE*);
    bool __fastcall isbmp(BYTE*);
	BYTE jpg[10]{0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
	BYTE png[4]{0x89, 0x50, 0x4E, 0x47};
	BYTE bmp[2]{0x42, 0x4D};
    HANDLE DiskOpen;
	HANDLE Timer;
	ULONG TotNumOfClusters;
	int ClusterSize;
	BYTE *Cluster;
protected:
	void __fastcall Execute();
public:
	ULONG NumberCluster;
    const char *Signature;
	__fastcall RCThread(bool CreateSuspended, std::wstring path);
	void __fastcall UpdatePB();
	void __fastcall EndOfThread();
    __fastcall ~RCThread();
};
//---------------------------------------------------------------------------
#endif
