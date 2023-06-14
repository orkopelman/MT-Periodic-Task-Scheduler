#include "scheduler.hpp"
using namespace threads;
using namespace std::chrono_literals; // std::this_thread::sleep_for


Scheduler::Scheduler() 
: m_threadPool(std::thread::hardware_concurrency())
, m_stop(false) 
{} 

Scheduler::~Scheduler() {
    m_stop = true;
}

void Scheduler::AddTimedTask(std::shared_ptr<TimedTask> a_TimedTask) {
    std::unique_lock<std::mutex> lock(m_mutex);
    AddTimedTaskinternal((a_TimedTask));
    m_cv.notify_one();
}

void Scheduler::Run() {
    auto startTime = std::chrono::system_clock::now();
    std::cout << "Time 0 : " << "nothing happen" <<  "\n";

    while (true) {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { return m_stop || !m_taskQueue.empty(); });
        if (m_stop && m_taskQueue.empty())
            return;

        m_currentTime = std::chrono::system_clock::now();

        while (!m_taskQueue.empty() && m_taskQueue.top()->m_exceTime <= m_currentTime) {
            std::shared_ptr<TimedTask> tempTimedTask = m_taskQueue.top();
            PrintTimeDif(startTime, m_currentTime);
            m_taskQueue.pop();

            if (ExcuteTask(tempTimedTask) == TIME_TASK_NEED_RESCHEDULE) {
                AddTimedTaskinternal((tempTimedTask));
            }
        }
    }
}

void Scheduler::StopExecution() {
    std::unique_lock<std::mutex> lock(m_mutex);
    size_t numOfTasks = m_taskQueue.size();
    while (!m_taskQueue.empty()) {
        m_taskQueue.pop();
    }
    m_stop = true;
    
    m_cv.notify_one(); // Notify the waiting thread to wake up
    std::cout << "Excution Stopped, " << numOfTasks << " task/s were in the queue when terminated \n";
    m_threadPool.ShutDown();
}

void Scheduler::AddTimedTaskinternal(std::shared_ptr<TimedTask> a_TimedTask) {
    a_TimedTask->setExcTime();
    m_taskQueue.push(a_TimedTask);
}

int Scheduler::ExcuteTask(std::shared_ptr<TimedTask> a_timedTask) {
    m_threadPool.AddTask(a_timedTask->m_task);
    if (a_timedTask->m_timesToPerform == ALWAYS_PERFORM) {
        return TIME_TASK_NEED_RESCHEDULE;
    }
    a_timedTask->m_timesToPerform--;
    return (a_timedTask->m_timesToPerform>0) ? TIME_TASK_NEED_RESCHEDULE : TIME_TASK_NEED_REMOVAL;
}

void Scheduler::PrintTimeDif(std::chrono::system_clock::time_point a_from, std::chrono::system_clock::time_point a_to) {
    auto elapsedTime = a_to - a_from;
    double elapsedTimeInSeconds = std::chrono::duration<double>(elapsedTime).count();
    std::cout << "Time " << elapsedTimeInSeconds << " ";
           
}

double Scheduler::CalcElpasedTime(std::chrono::system_clock::time_point a_from, std::chrono::system_clock::time_point a_to) {
    auto elapsedTime = a_to - a_from;
    return std::chrono::duration<double>(elapsedTime).count();
  
}