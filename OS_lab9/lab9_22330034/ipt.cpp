#include "../ipt.h"
void InvertedPageTable::setSID()
{
	sid = "22330034";//replace it with your student id
}

ADDRESS InvertedPageTable::lookup(int iPageNo, int iPID) const
{
	ADDRESS id = INVALID;
	ADDRESS ori_id = INVALID;
	ori_id = hash(iPageNo, iPID);
	id = ori_id;

	while (true)
	{
		if(vTable[id].inMemory() && vTable[id].hit(iPageNo, iPID))
			return id;
		else
			id = vTable[id].link;
		
		if(id == ori_id || id == INVALID)
			return INVALID;
	}
}

ADDRESS InvertedPageTable::insert(int iPageNo, int iPID)
{
	assert(lookup(iPageNo, iPID) == INVALID);//需要insert说明原来不在内存里

	ADDRESS id = INVALID;
	ADDRESS ori_id = INVALID;
	ori_id = hash(iPageNo, iPID);
	id = ori_id;

	while (true)
	{
		if(!vTable[id].inMemory())
		{
			vTable[id].set(iPageNo, iPID, 4);
			return id;
		}

		else if(vTable[id].hit(iPageNo, iPID))
			return id;

		else
		{
			ADDRESS pre_id = id;
			id = conflict(id);
			vTable[pre_id].link = id;
		}

		if(id == ori_id || id == INVALID)
			return INVALID;
	}
}