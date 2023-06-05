#ifndef SCHDULER_HPP
#define SCHDULER_HPP

#include "task.hpp"
#include <vector>
#include <queue>
#include <atomic>
#include <mutex>

class Schduler {
public:
    Schduler() : m_shouldContinue(true){}

    void AddTask(std::unique_ptr<Task> a_task);
    void Run();
    int PauseExcecution();
    int ResumeExcecution();

    bool getStatus() {return m_shouldContinue;};


private:
    bool NoTasksInserted() const;
    void SetAndAddTasks();

private:
    std::atomic_bool m_shouldContinue;
    //int m_timePaused;
    //int m_timeContinued;
    std::mutex m_mutex;
    std::vector<std::unique_ptr<Task>> m_vecTasks;
    std::priority_queue<std::unique_ptr<Task>,std::vector<std::unique_ptr<Task>>,LessThanByCrit> m_tasks;
};


#endif //__SCHDULER_HPP__