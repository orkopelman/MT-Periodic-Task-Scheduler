#include "schduler.hpp"
#include <thread>

// Schduler::Schduler()
// : m_shouldContinue(true)
// , m_vecTasks(std::vector<std::unique_ptr<Task>>{nullptr})
// , m_tasks(std::priority_queue<std::unique_ptr<Task>,std::vector<std::unique_ptr<Task>>,LessThanByCrit>{nullptr})
// {}


void Schduler::AddTask(std::unique_ptr<Task> a_task) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_vecTasks.push_back(std::move(a_task));
}

void Schduler::Run() {

    m_shouldContinue = true;
    if (NoTasksInserted()) return;
    
    // Thread for adding tasks to the heap
    std::thread addTaskThread([&]() {
        std::lock_guard<std::mutex> lock(m_mutex);
        SetAndAddTasks();
    });

 
    time_t startTime = time(0);
    std::cout << "Time 0 : " << "nothing happen" << ": \n";

    std::thread executeTaskThread([&]() {
        while (!m_tasks.empty() && m_shouldContinue) {
            if (m_tasks.top()->getExcTime() <= time(0)) {
                std::cout << "Time " << time(0) - startTime << " : ";
                std::unique_ptr<Task> tempTask;
                {
                    std::lock_guard<std::mutex> lock(m_mutex);
                    tempTask = std::make_unique<Task>(*(m_tasks.top()));
                    m_tasks.pop();
                }
                if (tempTask->execute() == TASK_NEED_RESCHDULE) {
                    tempTask->setExcTime();
                    std::lock_guard<std::mutex> lock(m_mutex);
                    m_tasks.push(std::move(tempTask));
                }
            }
            else {
                std::this_thread::sleep_for(std::chrono::seconds(m_tasks.top()->getExcTime() - time(0)));
            }
        }
    });

    addTaskThread.join();
    executeTaskThread.join();

    // while (!m_tasks.empty() && m_shouldContinue) {
        
    //     if (m_tasks.top()->getExcTime() <= time(0)) {
           
          
    //         std::cout << "Time " << time(0) -startTime  << " : ";
    //         std::unique_ptr<Task> tempTask = std::make_unique<Task>(*(m_tasks.top()));
    //         m_tasks.pop();
    //         if (tempTask->execute() == TASK_NEED_RESCHDULE) {
    //             tempTask->setExcTime();
    //             m_tasks.push(std::move(tempTask));
    //         }
    //     }
    //     else {
    //         std::this_thread::sleep_for(std::chrono::seconds(time(0) - m_tasks.top()->getExcTime()));
    //         }
    //     }       
}

int Schduler::PauseExcecution(){
    //time_t timePaused = time(0);
    m_shouldContinue = false;
    return m_tasks.size();
}

// int Schduler::ResumeExcecution(){
//     time_t timePaused = time(0);
//     m_shouldContinue = false;
//     return m_tasks.size();
// }

bool Schduler:: NoTasksInserted() const {
    return m_vecTasks.empty();
}

void Schduler::SetAndAddTasks() {
    for (auto& task : m_vecTasks) {
        task->setExcTime();
        m_tasks.push(std::move(task));
    }
}