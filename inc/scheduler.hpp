#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "task.hpp"
#include "threads_pool.hpp"
#include <vector>
#include <queue>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    void AddTimedTask(std::shared_ptr<TimedTask> a_TimedTask);
    void Run();
    void StopExecution();

private:
    void AddTimedTaskinternal(std::shared_ptr<TimedTask> a_TimedTask);
    int ExcuteTask(std::shared_ptr<TimedTask> a_timedTask);
    
    
private:
    std::priority_queue<std::shared_ptr<TimedTask>,std::vector<std::shared_ptr<TimedTask>>,CompaeExecTimes> m_taskQueue;
    threads::ThreadPool m_threadPool;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::atomic<bool> m_stop;
    std::chrono::system_clock::time_point m_currentTime;
                        
};




#endif //__SCHEDULER_HPP__