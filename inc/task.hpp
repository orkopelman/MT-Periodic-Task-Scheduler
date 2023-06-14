#ifndef __TASK_HPP__
#define __TASK_HPP__

#include "threads_pool_act.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <chrono>

constexpr int ALWAYS_PERFORM = -10;
constexpr int TIME_TASK_NEED_RESCHEDULE = 0;
constexpr int TIME_TASK_NEED_REMOVAL = 1;

using excTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<double>>;

class TimedTask {
public:
    TimedTask(std::string a_name, std::chrono::duration<double> a_interval, 
                std::shared_ptr<threads::AbstractAct> a_task, 
                int a_timesToPerform=ALWAYS_PERFORM);
               
    TimedTask(const TimedTask& other);
     
    void setExcTime();
    void printExcTimeInSeconds();

public:
    std::string m_name;
    std::chrono::duration<double> m_interval;
    std::shared_ptr<threads::AbstractAct> m_task;
    int m_timesToPerform;
    excTime m_exceTime; 
};



 struct CompaeExecTimes {
    bool operator()(const std::shared_ptr<TimedTask>& a_first,const std::shared_ptr<TimedTask>& a_sec) const
    {
        return a_first->m_exceTime > a_sec->m_exceTime;
    }

 };


#endif //__TASK_HPP__