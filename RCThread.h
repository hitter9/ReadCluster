//---------------------------------------------------------------------------

#ifndef RCThreadH
#define RCThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <chrono>
#include "RCThread2.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class RCThread : public TThread
{
private:
	bool __fastcall isjpg(BYTE Signature[]);
	bool __fastcall ispng(BYTE Signature[]);
    bool __fastcall isbmp(BYTE Signature[]);
	BYTE jpg[10]{0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
	BYTE png[4]{0x89, 0x50, 0x4E, 0x47};
	BYTE bmp[2]{0x42, 0x4D};
    HANDLE DiskOpen;
    const char *ErrorMessage;
	HANDLE Timer;
	ULONG TotNumOfClusters;
	int ClusterSize;
	BYTE *Cluster;
	string ressec;
    void __fastcall ShowErrMsg();
	void __fastcall UpdatePB();
	void __fastcall EndOfThread();
protected:
	void __fastcall Execute();
public:
	ULONG NumberCluster;
	ULONG NumClus;
	HANDLE SecThread;
	const char *FileType;
	const char *FiType;
	__fastcall RCThread(bool CreateSuspended, wstring path);
    __fastcall ~RCThread();
};
//---------------------------------------------------------------------------
#endif
