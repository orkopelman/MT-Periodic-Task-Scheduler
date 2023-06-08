#include "scheduler.hpp"
#include <thread>
#include <iomanip>

Schduler::Schduler()
: m_isPaused(false)
{}


void Schduler::AddTimedTask(std::unique_ptr<TimedTask> a_TimedTask) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_vecTimedTasks.push_back(std::move(a_TimedTask));
}


void Schduler::Run() {
    if (m_vecTimedTasks.empty()) return;
    
    std::thread coordinator([&]() {
        std::lock_guard<std::mutex> lock(m_mutex);
        SetAndAddTimedTasks();
        m_cv.notify_one(); 
    });

 
    auto startTime = std::chrono::system_clock::now();
    std::cout << "Time 0 : " << "nothing happen" <<  "\n";

    std::thread executeTimedTaskThread([&]() {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (!m_TimedTasks.empty()) {
            auto currentTime = std::chrono::system_clock::now();
            if (!m_isPaused && m_TimedTasks.top()->ExcTime() <= currentTime) {
                lock.unlock();  // Unlock before executing the task
                auto elapsedTime = currentTime - startTime;
                double elapsedTimeInSeconds = std::chrono::duration<double>(elapsedTime).count();
                std::cout << "Time " << elapsedTimeInSeconds << " ";
                std::unique_ptr<TimedTask> tempTimedTask;
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    tempTimedTask = std::make_unique<TimedTask>(*(m_TimedTasks.top()));
                    m_TimedTasks.pop();
                }
                if (tempTimedTask->execute() == TIME_TASK_NEED_RESCHEDULE) {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_TimedTasks.push(std::move(tempTimedTask));
                }
                lock.lock();  // Lock again after executing the task
            } else {
                if (m_TimedTasks.empty()) {
                    m_cv.wait(lock);  // Wait for a notification when there are no tasks
                } else {
                    auto elapsedTime = currentTime - m_TimedTasks.top()->ExcTime();
                    std::chrono::milliseconds sleepDuration(static_cast<int64_t>(std::chrono::duration<double>(elapsedTime).count() * 1000));
                    m_cv.wait_for(lock, sleepDuration);  // Wait for a specific duration or a notification
                }
            }
        }
    });
    coordinator.join();
    executeTimedTaskThread.join();


}

int Schduler::PauseExcecution(){
    m_isPaused = true; 
    return m_TimedTasks.size();
}

void Schduler::ResumeExcecution(){
    m_isPaused = false;
}

bool Schduler:: NoTimedTasksInserted() const {
    return m_vecTimedTasks.empty();
}

void Schduler::SetAndAddTimedTasks() {
    //std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& TimedTask : m_vecTimedTasks) {
        TimedTask->setExcTime();
        m_TimedTasks.push(std::move(TimedTask));
    }
    //m_vecTimedTasks.clear();

}