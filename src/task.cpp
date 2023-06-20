#include "task.hpp"

static std::chrono::_V2::system_clock::rep produceCurrentTimeInNanoSec() {
    std::chrono::system_clock::time_point tp = std::chrono::system_clock::now();
    std::chrono::system_clock::duration dtn = tp.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(dtn).count();
}

std::string getCurrentTimeAsString() {
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* timeInfo = std::localtime(&currentTime);
    int hours = timeInfo->tm_hour;
    int minutes = timeInfo->tm_min;
    int seconds = timeInfo->tm_sec;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours << ":"
       << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
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

void TimedTask::PrintExecutionMessage() const noexcept {
    std::cout << getCurrentTimeAsString();
    std::cout <<" : Task ID: " << m_taskID 
    << " Task Name: "<< m_name <<" was executed.\n";
}