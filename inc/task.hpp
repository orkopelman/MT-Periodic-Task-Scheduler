#ifndef __TASK_HPP__
#define __TASK_HPP__

#include "abstract_task.hpp"

#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

constexpr int ALWAYS_PERFORM = -10;
constexpr int TIME_TASK_NEED_RESCHEDULE = 0;
constexpr int TIME_TASK_NEED_REMOVAL = 1;

using excTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double>>;

class TimedTask {
public:
    TimedTask(std::chrono::duration<double> a_period, std::unique_ptr<AbstractTask> a_task, int a_timesToPerform=ALWAYS_PERFORM);
    TimedTask(TimedTask& a_other);
    

    int execute();
    void setExcTime();
    excTime ExcTime();

private:
    std::unique_ptr<AbstractTask> m_task;
    std::chrono::duration<double> m_periodic;
    int m_timesToPerform;
    excTime m_exceTime; 
};


struct CompaeExecTimes {
    bool operator()(const std::unique_ptr<TimedTask>& a_first,const std::unique_ptr<TimedTask>& a_sec) const
    {
        return a_first->ExcTime() > a_sec->ExcTime();
    }
};

#endif //__TASK_HPP__