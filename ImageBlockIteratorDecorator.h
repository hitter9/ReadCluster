//---------------------------------------------------------------------------

#ifndef ImageBlockIteratorDecoratorH
#define ImageBlockIteratorDecoratorH
//---------------------------------------------------------------------------
#include <iostream>
#include <windows.h>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class BlockIterator
{
private:
	int BSize;
	int CSize;
	int SSize;
	int Position;
protected:
	BYTE *BL;
	BYTE *CL;
public:
	BlockIterator(BYTE &Block, int BlockSize, int ClusterSize)
	{
		Position = 0;
		BSize = BlockSize;
		BL = &Block;
		CSize = ClusterSize;
		SSize = 10;
		CL = new BYTE[CSize];
	};
	virtual void First()
	{
		for (int i = 0; i < SSize; i++)
			CL[i] = BL[i];
		Position = CSize;
	};
	virtual void Next()
	{
		for (int i = 0; i < SSize; i++, Position++)
			CL[i] = BL[Position];
		Position += CSize - SSize;
	};
	virtual BYTE *GetCurrent()
	{
		return CL;
	};
	virtual bool IsDone()
	{
		if (Position > BSize)
			return true;
		else return false;
	};
	virtual ~BlockIterator()
	{
		delete[] CL;
	};
};

class ImageBlockIteratorDecorator : public BlockIterator
{
private:
	BYTE *CurrentCluster;
	BYTE jpg[10]{0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
	BYTE png[4]{0x89, 0x50, 0x4E, 0x47};
	BYTE bmp[2]{0x42, 0x4D};
	bool IsJPG(BYTE Signature[])
	{
		for (int i = 0; i < sizeof(jpg); i++)
		{
			if (Signature[i] != jpg[i])
				return false;
		}
		return true;
	};
	bool IsPNG(BYTE Signature[])
	{
		for (int i = 0; i < sizeof(png); i++)
		{
			if (Signature[i] != png[i])
				return false;
		}
		return true;
	};
	bool IsBMP(BYTE Signature[])
	{
		for (int i = 0; i < sizeof(bmp); i++)
		{
			if (Signature[i] != bmp[i])
				return false;
		}
		return true;
	};
public:
	ImageBlockIteratorDecorator(BYTE &Block, int BlockSize, int ClusterSize)
		: BlockIterator(Block, BlockSize, ClusterSize) {};
	virtual void First()
	{
		CurrentCluster = new BYTE[2];
		CurrentCluster[0] = 0;
		CurrentCluster[1] = 0;
		for (BlockIterator::First(); !BlockIterator::IsDone();
			BlockIterator::Next(), CurrentCluster[0]++)
		{
			CL = BlockIterator::GetCurrent();
			if (IsJPG(CL))
			{
				CurrentCluster[1] = 1;
				break;
			}
			if (IsPNG(CL))
			{
				CurrentCluster[1] = 2;
				break;
			}
			if (IsBMP(CL))
			{
				CurrentCluster[1] = 3;
				break;
			}
		}
	};
	virtual void Next()
	{
		BlockIterator::Next();
		for (; !BlockIterator::IsDone(); BlockIterator::Next(),
			CurrentCluster[0]++)
		{
			CL = BlockIterator::GetCurrent();
			if (IsJPG(CL))
			{
				CurrentCluster[0]++;
				CurrentCluster[1] = 1;
				break;
			}
			if (IsPNG(CL))
			{
				CurrentCluster[0]++;
				CurrentCluster[1] = 2;
				break;
			}
			if (IsBMP(CL))
			{
				CurrentCluster[0]++;
				CurrentCluster[1] = 3;
				break;
			}
		}
	};
	virtual BYTE* GetCurrent()
	{
		return CurrentCluster;
	};
	virtual bool IsDone()
	{
		return BlockIterator::IsDone();
	};
	virtual ~ImageBlockIteratorDecorator()
	{
		delete[] CurrentCluster;
	};
};
//---------------------------------------------------------------------------
#endif
