#include "../dispatch.h"
#include <algorithm>

class FCFS : public Dispatch {
private:
	queue<Process> ready_queue;

	void setName() {
		strMethod = "FCFS";
	}
	bool nowaitting() {
		return ready_queue.empty();
	}
	void insert(Process proNew, bool isNewProcess) {
		ready_queue.push(proNew);
	}
	bool select(Process& proSelcet) {
		if(nowaitting())
			return false;
		
		proSelcet = ready_queue.front();
		ready_queue.pop();
		return true;
	}
	bool goOnRunning(Process proCurrent){
		return getCurrentTimeStamp() - proCurrent.tSelected - proCurrent.Ts;
	}
};

class RR : public Dispatch {
public:
	void setQ(int _q)
	{
		q = _q;
	}
private:
	int q;
	queue<Process> ready_queue;

	void setName() {
		strMethod = "RR"+to_string(q);
	}
	bool nowaitting() {
		return ready_queue.empty();
	}
	void insert(Process proNew, bool isNewProcess) {
		ready_queue.push(proNew);
	}
	bool select(Process& proSelcet) {
		if(nowaitting())
			return false;
		
		proSelcet = ready_queue.front();
		ready_queue.pop();
		return true;
	}
	bool goOnRunning(Process proCurrent) {
		int timeRun = getCurrentTimeStamp() - proCurrent.tSelected;
		return (timeRun < q) && (timeRun + proCurrent.done < proCurrent.Ts);
	}
};

class SPN : public Dispatch {
private:
	struct compare{
		bool operator()(Process a, Process b){
			return a.Ts > b.Ts;
		}
	};
	priority_queue<Process, vector<Process>, compare>ready_queue;

	void setName() {
		strMethod = "SPN";
	}
	bool nowaitting() {
		return ready_queue.empty();
	}
	void insert(Process proNew, bool isNewProcess) {
		ready_queue.push(proNew);
	}
	bool select(Process& proSelcet) {
		if(nowaitting())
			return false;
		
		proSelcet = ready_queue.top();
		ready_queue.pop();
		return true;
	}
	bool goOnRunning(Process proCurrent) {
		return getCurrentTimeStamp() - proCurrent.tSelected - proCurrent.Ts;
	}
};

class SRT : public Dispatch {
private:
	vector<Process>ready_queue;

	void setName() {
		strMethod = "SRT";
	}
	bool nowaitting() {
		return ready_queue.empty();
	}
	void insert(Process proNew, bool isNewProcess) {
		ready_queue.push_back(proNew);
	}
	bool select(Process& proSelcet) {
		if(nowaitting())
			return false;
		
		double selectTime = 1e10;
		int selectIndex = -1;
		TimeStamp_TYPE currentTime = getCurrentTimeStamp();
		for(int i = 0; i < ready_queue.size(); i++){
			Process& p = ready_queue[i];
			double time = p.Ts - p.done;
			if(time < selectTime){
				selectTime = time;
				selectIndex = i;
			}
		}

		proSelcet = ready_queue[selectIndex];
		ready_queue.erase(ready_queue.begin() + selectIndex);
		return true;
	}
	bool goOnRunning(Process proCurrent) {
		return false;
	}
};

class HRRN : public Dispatch {
private:
	vector<Process> ready_queue;

	void setName() {
		strMethod = "HRRN";
	}
	bool nowaitting() {
		return ready_queue.empty();
	}
	void insert(Process proNew, bool isNewProcess) {
		ready_queue.push_back(proNew);
	}
	bool select(Process& proSelcet) {
		if(nowaitting())
			return false;
		
		double selectRatio = -1.0;
		int selectIndex = -1;
		TimeStamp_TYPE currentTime = getCurrentTimeStamp();
		for(int i = 0; i < ready_queue.size(); i++){
			Process& p = ready_queue[i];
			double ratio = static_cast<double>(currentTime - p.arrive) / p.Ts;
			if(ratio > selectRatio){
				selectRatio = ratio;
				selectIndex = i;
			}
		}

		proSelcet = ready_queue[selectIndex];
		ready_queue.erase(ready_queue.begin() + selectIndex);
		return true;
	}
	bool goOnRunning(Process proCurrent) {
		return getCurrentTimeStamp() - proCurrent.tSelected - proCurrent.Ts;
	}
};

class Feedback : public Dispatch {
public:
	Feedback(int _n=3) : n(_n),vRQ(_n+1),iLevel(-1),count(0) {	}
	
private:
	int n;
	int iLevel; //the current running process comes from which RQ.
	vector<queue<Process> > vRQ;
	int count; //how many waitting processes.

	void setName() {
		strMethod = "Feedback";
	}

	void insert(Process proNew, bool isNewProcess) {
		if (isNewProcess)
		{
			vRQ[0].push(proNew);
			++count;
		}
		else
		{
			assert(iLevel < n);
			vRQ[iLevel + 1].push(proNew);
			++count;
		}
	}

	bool nowaitting()
	{
		return count == 0;
	}

	bool select(Process& proSelcet) {
		bool bFound = false;
		for (int ii = 0; ii <= n; ++ii)
		{
			if (vRQ[ii].empty())
				continue;
			bFound = true;
			proSelcet = vRQ[ii].front();
			vRQ[ii].pop();
			--count;
			iLevel = ii;
			break;
		}
		return bFound;
	}

	bool goOnRunning(Process proCurrent) {
		bool bFinished = (proCurrent.done + (getCurrentTimeStamp() - proCurrent.tSelected) == proCurrent.Ts);
		if (bFinished)
		{
			return false;
		}
		
		if (iLevel == n)
			return true;

		HowLong_TYPE duration = getCurrentTimeStamp() - proCurrent.tSelected;
		return (duration < (1 << iLevel));
	}
};

void test()
{
	FCFS a;
	a.run();

	int q = 1;
	RR b;
	b.setQ(q);
	b.run();

	RR c;
	q = 4;
	c.setQ(q);
	c.run();

	SPN d;
	d.run();

	SRT e;
	e.run();

	HRRN f;
	f.run();
}

//please use this main function when submitting.
int main() {
	fout.open("result.txt");
	test();
	fout.close();
	return 0;
}