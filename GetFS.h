//---------------------------------------------------------------------------

#ifndef GetFSH
#define GetFSH
//---------------------------------------------------------------------------
#include <windows.h>
#include <string>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class FileSystem
{
protected:
	BYTE BRec[512];
public:
	int ClusterSize;
	ULONGLONG TotNumOfClusters;
	LARGE_INTEGER StartOffset;
	FileSystem(BYTE BootRec[])
	{
		for (int i = 0; i < sizeof(BRec); i++)
			BRec[i] = BootRec[i];
	};
	virtual void FileSystemInfo() = 0;
	virtual ~FileSystem() {};
};

class NTFS_FileSystem : public FileSystem
{
public:
	NTFS_FileSystem(BYTE BootRec[]) : FileSystem(BootRec)
	{
		FileSystemInfo();
	};
	void FileSystemInfo()
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE reserved[11];
			UINT16 SectorSize;
			UINT8 ClusterMultiplier;
			BYTE reserved2[26];
			UINT64 NumberSector;
		} BOOT_NTFS;
		#pragma pack(pop)
		BOOT_NTFS *BootRecord;
		BootRecord = (BOOT_NTFS*)BRec;
		ClusterSize = BootRecord->SectorSize * BootRecord->ClusterMultiplier;
		TotNumOfClusters = BootRecord->NumberSector / BootRecord->ClusterMultiplier;
		StartOffset.QuadPart = 0;
	};
	~NTFS_FileSystem() {};
};

class exFAT_FileSystem : public FileSystem
{
public:
	exFAT_FileSystem(BYTE BootRec[]) : FileSystem(BootRec)
	{
		FileSystemInfo();
	};
	void FileSystemInfo()
	{
		#pragma pack(push, 1)
		typedef struct
		{
			BYTE reserved[88];
			UINT32 FirstBitmapSector;
			UINT32 NumberCluster;
			UINT32 RootDirectoryCluster;
			BYTE reserved2[8];
			BYTE SectorSize;
			BYTE ClusterMultiplier;
		} BOOT_exFAT;
		#pragma pack(pop)
		BOOT_exFAT *BootRecord;
		BootRecord = (BOOT_exFAT*)BRec;
		int SectorSize = pow(2, int(BootRecord->SectorSize));
		int ClusterMultiplier = pow(2, BootRecord->ClusterMultiplier);
		int FirstBitmapSector = BootRecord->FirstBitmapSector;
		int RootDirectoryCluster =  BootRecord->RootDirectoryCluster;
		ClusterSize = SectorSize * ClusterMultiplier;
		TotNumOfClusters = BootRecord->NumberCluster;
		StartOffset.QuadPart = FirstBitmapSector * SectorSize;
	};
	~exFAT_FileSystem() {};
};

class GetFileSystem
{
private:
	FileSystem *FS;
public:
	const char *ErrorMessage;
	int ClusterSize;
	ULONGLONG TotNumOfClusters;
	LARGE_INTEGER StartOffset;
	GetFileSystem(BYTE BootRec[])
	{
		ErrorMessage = NULL;
		BYTE NameFS[8];
		BYTE NTFS[8]{0xEB, 0x52, 0x90, 0x4E, 0x54, 0x46, 0x53, 0x20};
		BYTE exFAT[8]{0xEB, 0x76, 0x90, 0x45, 0x58, 0x46, 0x41, 0x54};
		int res;
		for (int i = 0; i < sizeof(NameFS); i++)
		{
			NameFS[i] = BootRec[i];
			if (NameFS[i] == NTFS[i])
				res = 1;
			else if (NameFS[i] == exFAT[i])
				res = 2;
			else
			{
				res = 0;
				break;
			}
		}
		switch (res)
		{
		case 1:
			FS = new NTFS_FileSystem(BootRec);
			ClusterSize = FS->ClusterSize;
			TotNumOfClusters = FS->TotNumOfClusters;
			StartOffset = FS->StartOffset;
			break;
		case 2:
			FS = new exFAT_FileSystem(BootRec);
			ClusterSize = FS->ClusterSize;
			TotNumOfClusters = FS->TotNumOfClusters;
			StartOffset = FS->StartOffset;
			break;
		default:
            FS = 0;
			ErrorMessage = "Файловая система не поддерживается";
		}
	}
	virtual ~GetFileSystem()
	{
		delete FS;
	}
};
//---------------------------------------------------------------------------
#endif
