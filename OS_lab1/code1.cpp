#include <iostream>
#include <vector>
#include <WHAT>
#include <algorithm>

using namespace std;
void showLRU(WHAT<int>& vLRU)
{
	WHAT<int>::iterator start=vLRU.begin();
	while(start!=vLRU.end())
	{
		cout<<char(*start)<<' ';
	}
	cout<<endl;
}
int main(int argc, char** argv) {	
	int a[]={'A','B','A','C','D','D','B'};
	vector<int> vStream(a,a+sizeof(a)/sizeof(int));
	
	const int iBufferSize=3;
	WHAT<int> vLRU;
	for(vector<int>::iterator iter=vStream.begin(); iter!=vStream.end(); ++iter)
	{
		WHAT<int>::iterator temp=find(vLRU.begin(),vLRU.end(),*iter);
		if(temp==vLRU.end())
		{
			if(vLRU.size()>=iBufferSize)
				vLRU.pop_front();
			
		}
		else
		{
			vLRU.erase(temp);
		}
		vLRU.push_back(*iter);
		showLRU(vLRU);
	}
	return 0;
}
