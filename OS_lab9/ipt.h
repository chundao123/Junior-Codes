#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <cassert>

using namespace std;
const int INVALID = -1;
typedef int ADDRESS;
const int PControl = 2;//假设控制位P在右起第2位

class ControlBits {
public:
	ControlBits() :iControl(0) {}
	ControlBits(int control) { iControl = (INVALID == control ? 0 : control); }

	void set(int _control)
	{
		iControl = _control;
	}
	//读右起第iWhich位
	bool getBit(int iWhich) const {
		return (iControl & (1 << iWhich));
	}
	//将右起第iWhich位设为1
	void setBit(int iWhich) {
		iControl = (iControl | (1 << iWhich));
	}
	//将右起第iWhich位设为0
	void resetBit(int iWhich) {
		iControl = (iControl & (~(1 << iWhich)));
	}
private:
	int iControl;
};

class PageItem {
public:
	PageItem() :iControl(0), link(INVALID) {}
	ADDRESS link;

	void set(int _iPageNo, int _iPID)
	{
		iPageNo = _iPageNo;
		iPID = _iPID;
		iControl.set(0);
		iControl.setBit(PControl);
	}

	void set(int _iPageNo, int _iPID, int _iControl)
	{
		iPageNo = _iPageNo;
		iPID = _iPID;
		iControl.set(_iControl);
	}

	bool hit(int _iPageNo, int _iPID) const
	{
		return iPageNo == _iPageNo && iPID == _iPID;
	}

	bool inMemory() const
	{
		return iControl.getBit(PControl);
	}

	void reset(int iWhich)
	{
		iControl.resetBit(iWhich);
	}
private:

	int iPageNo;
	int iPID;
	ControlBits iControl;
};

class InvertedPageTable {
public:
	vector<PageItem> vTable;
	string sid;
	void setSID();

	void initial(int _m) {
		m = _m;
		vTable.resize(1 << m);
	}

	ADDRESS lookup(int iPageNo, int iPID) const;

	ADDRESS insert(int iPageNo, int iPID);

	bool release(ADDRESS curr)
	{
		if (INVALID == curr)
			return false;
		vTable[curr].reset(PControl);
		return true;
	}
	bool release(int iPageNo, int iPID)
	{
		ADDRESS curr = lookup(iPageNo, iPID);
		return release(curr);
	}

private:

	int m;

	ADDRESS hash(int iPageNo, int iPID) const {
		return (iPageNo + (iPID << 2)) & ((1 << m) - 1);
	}

	ADDRESS conflict(ADDRESS curr) const {
		return ((curr + ((1 << m) >> 2))) & ((1 << m) - 1);
	}
};
