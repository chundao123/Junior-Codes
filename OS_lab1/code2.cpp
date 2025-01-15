#include <iostream>
#include <vector>

using namespace std;

void remove()
{
	int a[]={'A','A','C','D','D','D','D'};
	vector<int> vStream(a,a+sizeof(a)/sizeof(int));
	if(vStream.size()<2)
		return;
	
	int iPrev=vStream[0];
	for(vector<int>::iterator iter=vStream.begin()+1; iter!=vStream.end();++iter)
	{
		if(*iter==iPrev)
			vStream.erase(iter);
		else
		{
			iPrev=*iter;
		}
	}
	
	for(vector<int>::iterator iter=vStream.begin(); iter!=vStream.end();++iter)
		cout<<char(*iter);
}
int main(int argc, char** argv) {	
	remove();
	return 0;
}
