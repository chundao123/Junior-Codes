#include "../buddy.h"

ADDRESS Buddy::get_hole(int i) {
	//return the start position of the hole allocated to the process.
	//start writting your code from here
	if(i == U+1)
		return INVALID;
	
	if(!vArrayOfList[i].empty())
	{
		auto it = vArrayOfList[i].begin();
		ADDRESS start = *it;
		vArrayOfList[i].erase(it);
		return start;
	}

	if(i < U)
	{
		ADDRESS start = get_hole(i+1);
		if(start != INVALID)
		{
			ADDRESS buddystart = start + (1 << i);
			vArrayOfList[i].push_back(buddystart);
			return start;
		}
	}

	return INVALID;
	//end of your code
}

//NO modification is allowed below.
int main() {
	test();
	return 0;
}
