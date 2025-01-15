#include<iostream>
#include<fstream>
#include<sstream>
#include<deque>
#include<vector>

using namespace std;

//进程信息表
struct Process
{
    int id, directives;                 //进程名，进程剩余指令数
    unsigned int wait_length;           //等待队列长度
    unsigned int* wait_id = NULL;       //等待队列
    bool is_ready;                      //是否就绪

    Process(int _id, int _directives, int _wait_length, unsigned int* _wait_id) :id(_id), directives(_directives), wait_length(_wait_length), wait_id(_wait_id)
    {
        if (wait_length)
            is_ready = false;
        else
            is_ready = true;
    }

    void Show()
    {
        cout << "porcess id = " << id << "\tdirectives = " << directives << "\twait_length = " << wait_length;
        if (wait_length)
            cout << "\twait_id =";
        for (int i = 0; i < wait_length; i++)
            cout << ' ' << wait_id[i];
        cout << endl;
    }
};

class OS
{
private:

    string user_name;                   //用户名
    int operate_num, operate_time;      //m和n
    deque<Process> ready, blocking;     //就绪、阻塞队列
    ifstream f;                         //process.txt
    bool is_read_done = false;          //是否读完txt
    vector<Process> diary;              //运行日志，记录顺序

public:

    OS(string _user_name) :user_name(_user_name)
    {
        cout << "Welcome " << user_name << "!" << endl;
    }

    void PushProcess(Process p)
    {
        if (p.is_ready)
        {
            cout << "\nPush process into ready:\n";
            ready.push_back(p);
        }
        else
        {
            cout << "\nPush process into blocking:\n";
            blocking.push_back(p);
        }
        p.Show();
    }

    void ReadProcess()
    {
        //读取process.txt中的一行
        string line;
        getline(f, line);
        if (line == "")
        {
            is_read_done = true;
            return;
        }
        istringstream stream(line);
        unsigned int id;
        int directives, wait_num;
        unsigned int* wait_id = NULL;
        
        stream >> id;
        stream >> directives;
        stream >> wait_num;
        wait_id = new unsigned int[wait_num];
        for (int i = 0; i < wait_num; i++)
            stream >> wait_id[i];
        //转化成进程并储存
        Process p = Process(id, directives, wait_num, wait_id);
        cout << "\nRead process:\n";
        p.Show();
        PushProcess(p);
    }

    void CheckBlocking()
    {
        for (int i = 0; i < blocking.size(); i++)
        {
            Process blocking_front = blocking.front();
            blocking.pop_front();
            int finish = 0;
            for (int j = 0; j < blocking_front.wait_length; j++)
                for (int k = 0; k < diary.size(); k++)
                    if (blocking_front.wait_id[j] == diary[k].id && !diary[k].directives)
                    {
                        finish++;
                        break;
                    }
            if (finish == blocking_front.wait_length)
                blocking_front.is_ready = true;
            PushProcess(blocking_front);
        }
    }

    void Set()
    {
        f.open("process.txt");
        f >> operate_num >> operate_time;
        //cout << process_num << ' ' << operate_num << endl;
        string line;
        getline(f, line);
        //加载直到第一个可执行进程
        while (ready.empty())
            ReadProcess();
    }

    void Run()
    {
        while (!ready.empty())
        {
            //就绪队列不足则读取新进程
            if (ready.size() < operate_num && !blocking.empty())
                CheckBlocking();
            while (ready.size() < operate_num && !is_read_done)
                ReadProcess();
            //运行就绪队列头进程
            Process ready_front = ready.front();
            ready.pop_front();
            ready_front.directives -= operate_time;
            if (ready_front.directives > 0)
                ready.push_back(ready_front);
            else
                ready_front.directives = 0;
            //更新日志
            diary.push_back(ready_front);
            cout << "\nRun process:\n";
            ready_front.Show();
            //检测阻塞队列
            CheckBlocking();
        }
    }

    void Save()
    {
        ofstream o;
        o.open("result.txt", ios::out);
        vector<Process>::iterator it = diary.begin();
        while (1)
        {
            o << (*it++).id;
            if (it != diary.end())
                o << ",";
            else
                break;
        }
    }
};

int main()
{
    OS hujinhainb = OS("hujinhainb");
    hujinhainb.Set();
    hujinhainb.Run();
    hujinhainb.Save();
    return 0;
}