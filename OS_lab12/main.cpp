#include "DiskScheduler.h"
#include <fstream>
// 示例测试函数
int main() {
    std::vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};

    // 使用FIFO调度算法进行测试
    DiskScheduler* myScheduler;
    myScheduler =  new FIFODiskScheduler(requests);
    myScheduler->execute();
    std::string result=myScheduler->printResult();
    std::ofstream fout1("FIFO.txt", std::ios::app);
    fout1 << result;
    fout1.close();


    //测试代码需取消下方注释

    //myScheduler =  new SCANDiskScheduler(requests);
    //myScheduler->execute();
    //std::string result=myScheduler->printResult();
    //std::ofstream fout2("SCAN.txt", std::ios::app);
    //fout2 << result;
    //fout2.close();
    return 0;
}
