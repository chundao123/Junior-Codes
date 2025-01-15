#include <iostream>
#include <sstream>
#include <vector>

class DiskScheduler {
public:
    DiskScheduler(std::vector<int> requests, int initialPosition=0) : sid("mysid"),initialPosition_(initialPosition), requests_(std::move(requests)) {}
    std::string sid;

    virtual void execute() = 0;  // 纯虚函数，子类需实现具体的调度算法

    std::string printResult() const {
        
        std::cout << "Order of execution: ";
        std::stringstream result;
        result << sid << '\t';
        for (int track : orderOfExecution_) {
            result << track << '\t';
        }
        std::cout << result.str();
        std::cout << "\nTotal head movements: " << totalHeadMovements_ << std::endl;
        result << totalHeadMovements_ << std::endl;
        return result.str();
    }

protected:
    int initialPosition_;                 // 磁头初始位置
    std::vector<int> requests_;           // 磁道请求序列
    std::vector<int> orderOfExecution_;   // 执行顺序
    int totalHeadMovements_{0};           // 总磁头移动数
};

class FIFODiskScheduler : public DiskScheduler {
public:
    FIFODiskScheduler(std::vector<int> requests) : DiskScheduler(std::move(requests)) {}

    void execute() override {
        for (int track : requests_) {
            orderOfExecution_.push_back(track);
            totalHeadMovements_ += std::abs(track - currentHead_);
            currentHead_ = track;
        }
    }

private:
    int currentHead_{0};  // 当前磁头位置
};

// 待实现的类：
// SCANDiskScheduler 扫描算法

