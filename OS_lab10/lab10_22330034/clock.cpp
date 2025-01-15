#include "../replacement.h"
bool ClockPolicy::visitPage(PageItem* pi)
{
	//write your code here
	for(int i = 0; i < iBufferSize; i++)
		if(vBuffer[i] != NULL && vBuffer[i]->iPageNo == pi->iPageNo)
		{
			vBuffer[i]->iControl.setBit(PControl);
			vBuffer[i]->iControl.setBit(UControl);
			vBuffer[i]->iVisitTime = iCurrent;

			return false;
		}

	for(int i = 0; i < iBufferSize; i++)
		if(vBuffer[i] == NULL)
		{
			vBuffer[i] = new PageItem(pi->iPageNo);
			vBuffer[i]->iControl.setBit(PControl);
			vBuffer[i]->iControl.setBit(UControl);
			vBuffer[i]->iVisitTime = iCurrent;
			iCurrent = (iCurrent + 1) % iBufferSize;

			return true;
		}

	int replace_index = -1;
	for(int i = 0; i < iBufferSize; i++)
	{
		if(!vBuffer[i]->iControl.getBit(UControl))
		{
			replace_index = i;
			break;
		}

		else
			vBuffer[i]->iControl.resetBit(UControl);
	}

	if(replace_index == -1)
		replace_index = iCurrent;

	vBuffer[replace_index]->iPageNo = pi->iPageNo;
	vBuffer[replace_index]->iControl = pi->iControl;
	vBuffer[replace_index]->iControl.setBit(UControl);
	vBuffer[replace_index]->iVisitTime = iCurrent;
	iCurrent = (iCurrent + 1) % iBufferSize;

	return true;
}