#ifndef __TASK_HPP__
#define __TASK_HPP__

#include "abstract_task.hpp"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>

constexpr int FACTOR_NANO_TO_MSEC = 1000000;
constexpr int FACTOR_SEC_TO_MSEC = 1000;
constexpr int FACTOR_MICRO_TO_MILI = 1000;
constexpr int ALWAYS_PERFORM = -10;
constexpr int TASK_NEED_RESCHDULE = 0;
constexpr int TASK_NEED_REMOVAL = 1;

class Task {
public:
    Task(size_t a_period, std::unique_ptr<AbstractTask> a_task, int a_timesToPerform=ALWAYS_PERFORM);
    Task(Task& a_other);
    

    int execute();

    void setExcTime();
    time_t getExcTime();

private:
    std::unique_ptr<AbstractTask> m_task;
    size_t m_periodic;
    int m_timesToPerform;
    time_t m_exceTime; 
};

struct LessThanByCrit {
    bool operator()(const std::unique_ptr<Task>& a_first,const std::unique_ptr<Task>& a_sec) const
    {
        return a_first->getExcTime() > a_sec->getExcTime();
    }
};

#endif //__TASK_HPP__