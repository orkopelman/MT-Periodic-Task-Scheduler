#include "task.hpp"

TimedTask::TimedTask(std::chrono::duration<double> a_period, std::unique_ptr<AbstractTask> a_task,int a_timesToPerform)
: m_task(std::move(a_task)) 
, m_periodic(a_period)
, m_timesToPerform(a_timesToPerform)
//, m_exceTime(a_period)
{}

TimedTask::TimedTask(TimedTask& a_other) {
    m_task = std::move(a_other.m_task);
    m_periodic = a_other.m_periodic;
    m_timesToPerform = a_other.m_timesToPerform;
//    m_exceTime = a_other.m_exceTime;
}
    

void TimedTask::setExcTime() {
    m_exceTime = std::chrono::system_clock::now() + m_periodic; 
}

excTime TimedTask::ExcTime() {
    return m_exceTime;
}

int TimedTask::execute() {
    m_task->Act();
    setExcTime();
    if (m_timesToPerform != ALWAYS_PERFORM) {
        m_timesToPerform--;
    }
    return (m_timesToPerform != 0)? TIME_TASK_NEED_RESCHEDULE : TIME_TASK_NEED_REMOVAL; 
}


