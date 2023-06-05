#include "task.hpp"



Task::Task(size_t a_period, std::unique_ptr<AbstractTask> a_task,int a_timesToPerform)
: m_task(std::move(a_task)) 
, m_periodic(a_period)
, m_timesToPerform(a_timesToPerform)
{}

Task::Task(Task& a_other) {
    m_task = std::move(a_other.m_task);
    m_periodic = a_other.m_periodic;
    m_timesToPerform = a_other.m_timesToPerform;
    m_exceTime = a_other.m_exceTime;
}
    

void Task::setExcTime() {
    m_exceTime = time(0) + m_periodic;  
}

time_t Task::getExcTime() {
    return m_exceTime;
}

int Task::execute() {
    m_task->Act();
    if (m_timesToPerform != ALWAYS_PERFORM) {
        m_timesToPerform--;
    }
    return (m_timesToPerform != 0)? TASK_NEED_RESCHDULE : TASK_NEED_REMOVAL; 
}


// struct LessThanByCrit {
//     bool operator()(const std::unique_ptr<Task>& a_first,const std::unique_ptr<Task>& a_sec) const
//     {
//         return a_first->getExcTime() < a_sec->getExcTime();
//     }
// };