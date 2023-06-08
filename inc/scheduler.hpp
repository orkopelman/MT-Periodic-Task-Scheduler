#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include "task.hpp"
#include <vector>
#include <queue>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>

class Schduler {
public:
    Schduler();

    void AddTimedTask(std::unique_ptr<TimedTask> a_TimedTask);
    void Run();
    int PauseExcecution();
    void ResumeExcecution();
 

private:

    bool NoTimedTasksInserted() const;
    void SetAndAddTimedTasks();

private:
    std::atomic_bool m_isPaused;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::vector<std::unique_ptr<TimedTask>> m_vecTimedTasks;
    std::priority_queue<std::unique_ptr<TimedTask>,std::vector<std::unique_ptr<TimedTask>>,CompaeExecTimes> m_TimedTasks;
};


#endif //__SCHEDULER_HPP__