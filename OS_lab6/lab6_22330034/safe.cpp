#include "banker.h"
void State::setStudentID()
{
	studentID="22330034";//replace it with your real student id
}
bool State::safe()
{
	bool possible = true;
	//write your code here
	int count = 0;
	vector<bool> finish(numOfProcess, false);
	vector<int> current_available = available;
	while (count != numOfProcess)
	{
		//检测哪个进程可以释放
		bool can_release;
		for (int i = 0; i < numOfProcess; i++)
		{
			//若该进程已释放则退出
			if (finish[i])
				continue;

			can_release = true;
			//检测每个资源是否全部足够
			for (int j = 0; j < numOfResource; j++)
				//若有资源不能释放则跳过
				if (claim[i][j] > current_available[j] + alloc[i][j])
				{
					can_release = false;
					break;
				}

			//释放进程
			if (can_release)
			{
				for (int j = 0; j < numOfProcess; j++)
					current_available[j] += alloc[i][j];
				finish[i] = true;
				count++;
			}
		}
		//若没有进程资源可释放，则产生死锁不安全
		if (!can_release)
		{
			possible = false;
			break;
		}
	}
	//DO NOT output anything inside this function
	return possible;
}
