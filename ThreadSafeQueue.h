//---------------------------------------------------------------------------

#ifndef ThreadSafeQueueH
#define ThreadSafeQueueH
//---------------------------------------------------------------------------
#include <Windows.h>
#include <queue>
#include <utility>
#include <mutex>
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
class ThreadSafeQueue
{
private:
	typedef pair<ULONG, const char*> entry;
	queue<entry> rawQueue;
	mutex m;
public:
	entry frontpop()
	{
		lock_guard<mutex> lock(m);
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
		lock_guard<mutex> lock(m);
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
