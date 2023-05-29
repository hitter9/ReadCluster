//---------------------------------------------------------------------------

#ifndef RCThreadH
#define RCThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <chrono>
#include "GetFS.h"
#include "ThreadSafeQueue.h"
#include "ImageBlockIteratorDecorator.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class RCThread : public TThread
{
private:
	HANDLE DiskOpen;
    const char *ErrorMessage;
	HANDLE Timer;
	ULONGLONG TotNumOfClusters;
	int ClusterSize;
    LARGE_INTEGER StartOffset;
	BYTE *Block;
   	ULONG NumberCluster;
	const char *FileType;
	string ResSec;
    void __fastcall ShowErrMsg();
	void __fastcall UpdatePB();
	void __fastcall EndOfThread();
protected:
	void __fastcall Execute();
public:
	ThreadSafeQueue TSQ;
	__fastcall RCThread(bool CreateSuspended, wstring path);
    __fastcall ~RCThread();
};
//---------------------------------------------------------------------------
#endif
