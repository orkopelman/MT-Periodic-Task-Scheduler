#include "task.hpp"

TimedTask::TimedTask(std::string a_name, std::chrono::duration<double> a_interval, std::shared_ptr<threads::AbstractAct> a_task,
                        int a_timesToPerform)
: m_name(a_name)
, m_interval(a_interval)
, m_task((a_task)) 
, m_timesToPerform(a_timesToPerform)
, m_exceTime(a_interval)
{}

TimedTask::TimedTask(const TimedTask& a_other) {
    m_name = a_other.m_name;
    m_interval = a_other.m_interval;
    m_task = a_other.m_task;
    m_timesToPerform = a_other.m_timesToPerform;
}


void TimedTask::printExcTimeInSeconds() {
    auto duration = m_exceTime.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration).count();
    std::cout << m_name<< " ExcTime in seconds: " << seconds << std::endl;
}

void TimedTask::setExcTime() {
    m_exceTime = std::chrono::system_clock::now() + m_interval;
}