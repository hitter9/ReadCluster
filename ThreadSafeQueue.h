//---------------------------------------------------------------------------

#ifndef ThreadSafeQueueH
#define ThreadSafeQueueH
//---------------------------------------------------------------------------
#include <Windows.h>
#include <queue>
#include <utility>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class ThreadSafeQueue
{
private:
	typedef pair<ULONG, const char*> entry;
	queue<entry> rawQueue;
public:
	entry frontpop()
	{
		pair<ULONG, const char*> front_value (NULL, NULL);
		if(!rawQueue.empty())
		{
			front_value = rawQueue.front();
			rawQueue.pop();
		}
		return front_value;
	};
	void push(entry val)
	{
		rawQueue.push(val);
	};
	bool empty()
	{
		if (rawQueue.empty())
			return true;
		else return false;
	};
};
//---------------------------------------------------------------------------
#endif
