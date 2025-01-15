#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <bitset>
#include <map>
#include <algorithm>


const int U = 6; //usually, the total size of memory is known.

using namespace std;
typedef size_t ADDRESS;
typedef char PIDTYPE;
const ADDRESS INVALID = -1;

class Buddy {
public:
	Buddy(int _L = 0) : L(_L), vArrayOfList(U + 1), iMaxSize(1 << U) {
		memory.reset();
		vArrayOfList[U].push_back(0);
	}

	void apply(size_t applySize, PIDTYPE PID)
	{
		int i = 0;
		while (applySize > (1 << i))
			++i;

		ADDRESS result = get_hole(i);
		if (result!=INVALID)
		{
			size_t iSize = 1 << (i < L ? L : i);
			for (size_t ii = 0; ii < iSize; ++ii)
				memory.set(result + ii, 1);
			show();
			mAllocated[PID]=pair<ADDRESS, int>(result, i);
		}
		else
			cout << "Failure" << endl;
	}

	void release(PIDTYPE PID)
	{
		map<PIDTYPE, pair<ADDRESS, int> >::iterator iter = mAllocated.find(PID);
		if (iter == mAllocated.end())
		{
			cout << "no allocated" << endl;
			return;
		}
		ADDRESS start = iter->second.first;
		int i = iter->second.second;
		mAllocated.erase(iter);

		//reset
		size_t iSize = 1 << i;
		if (memory[start]) {
			for (size_t ii = 0; ii < iSize; ++ii)
			{
				memory.reset(start + ii);
			}
		}
		vArrayOfList[i].push_back(start);
		checkMerge(start, i);			
		show();
	}

private:
	int L;
	int iMaxSize;
	bitset<1 << U> memory;
	vector<list<ADDRESS> > vArrayOfList;

	map<PIDTYPE, pair<ADDRESS, int> > mAllocated;

	void show()
	{
		//return;
		for (int ii = 0; ii < iMaxSize; ++ii)
			cout << memory[ii];
		cout << endl;
	}
	ADDRESS get_hole(int i);

	bool checkMerge(ADDRESS start, int i)
	{
		size_t iSize = 1 << i;

		//seek for merge
		bool bMerge = false;
		if (start >= iSize)
		{
			ADDRESS left = start - iSize;
			if (!memory[left])
			{
				list<ADDRESS>::iterator iter = find(vArrayOfList[i].begin(), vArrayOfList[i].end(), left);
				if (iter != vArrayOfList[i].end())
				{
					vArrayOfList[i].erase(iter);
					vArrayOfList[i + 1].push_back(left);
					iter = find(vArrayOfList[i].begin(), vArrayOfList[i].end(), start);
					vArrayOfList[i].erase(iter);
					checkMerge(left, i + 1);
					bMerge = true;
				}
			}
		}
		//if (bMerge) return true;
		if (start + iSize < iMaxSize)
		{
			ADDRESS right = start + iSize;
			if(!memory[right])
			{
				list<ADDRESS>::iterator iter = find(vArrayOfList[i].begin(), vArrayOfList[i].end(), right);
				if (iter != vArrayOfList[i].end())
				{
					vArrayOfList[i].erase(iter);
					vArrayOfList[i + 1].push_back(start);
					iter = find(vArrayOfList[i].begin(), vArrayOfList[i].end(), start);
					vArrayOfList[i].erase(iter);
					checkMerge(start, i + 1);
					bMerge = true;
				}
			}
		}
		return bMerge;
	}
};



void testApply() {
	Buddy bb(0);
	int testSizes[7] = { 8,30,17,1,32,2,4 };
	for (int ii = 0; ii < 7; ++ii)
		bb.apply(testSizes[ii], 'A' + ii);
}
void testRelease() {
	Buddy bb(0);
	int testSizes[7] = {8,30,17,1,32,2,4};
	for (int ii = 0; ii < 7; ++ii)
		bb.apply(testSizes[ii],'A'+ii);

	for (int ii = 0; ii < 7; ++ii)
		bb.release('A'+ii);
	cout << endl << "the following output should be same as above." << endl;
	for (int ii = 0; ii < 7; ++ii)
		bb.apply(testSizes[ii], 'A' + ii);

	for (int ii = 0; ii < 7; ++ii)
		bb.release('A' + ii);
}

void test() {
	testApply();
}

