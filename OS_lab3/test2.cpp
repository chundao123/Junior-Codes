#include <iostream>
#include <thread>
#include <vector>
#include <unistd.h>
using namespace std;



class myClass {
public:

	void print_block(int c) {

		cout << "PrintTimes=" << iPrintTimes << endl;
		for (int i = 0; i < iPrintTimes; ++i) { cout << c; usleep(1);}//sleep for 1 microsecond
		cout << '\n';

	}
	void setPrintTimes(int times)
	{
		iPrintTimes = times;
	}
private:
	int iPrintTimes;
};

void tryThreads()
{
	myClass class0;
	class0.setPrintTimes(10);
	myClass class1;
	class1.setPrintTimes(20);
	myClass class2;
	class2.setPrintTimes(30);
	thread th0(&myClass::print_block, &class0, 0);
	thread th1(&myClass::print_block, &class1, 1);
	thread th2(&myClass::print_block, &class2, 2);

	th0.join();
	th1.join();
	th2.join();
}

int main()
{
	tryThreads();
	return 0;
}
