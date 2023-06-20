#include "task.hpp"

static std::chrono::_V2::system_clock::rep produceCurrentTimeInNanoSec() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dtn = tp.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(dtn).count();
}


TimedTask::TimedTask(const std::string& a_name, std::chrono::duration<double> a_interval, std::shared_ptr<threads::AbstractAct> a_task,
                        int a_timesToPerform)
: m_taskID(produceCurrentTimeInNanoSec())
, m_name(a_name)
, m_interval(a_interval)
, m_task((a_task)) 
, m_timesToPerform(a_timesToPerform)
, m_exceTime(a_interval)
{}

void TimedTask::setExcTime() {
    m_exceTime = std::chrono::system_clock::now() + m_interval;
}

excTime TimedTask::ExcTime() const noexcept {
    return m_exceTime;
}

std::chrono::_V2::system_clock::rep TimedTask::TaskId() const noexcept{
    return m_taskID;
}

std::shared_ptr<threads::AbstractAct> TimedTask::Task() const noexcept{
    return m_task;
}

int TimedTask::TimesToPerform() const noexcept {
    return m_timesToPerform;
}

void TimedTask::updateTimeToPerofm() {
    m_timesToPerform--;
}