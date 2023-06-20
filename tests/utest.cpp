#include "mu_test.h"
#include "scheduler.hpp"
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std::chrono_literals; // std::this_thread::sleep_for

using namespace threads;

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

struct PrintStrAct : public AbstractAct {
	PrintStrAct(std::string a_str) : m_str(a_str) {}
	void Act() override {
        std::cout << m_str  << " at: " << getCurrentTimeAsString() << "\n";
    }

 	std::string m_str;
};

std::shared_ptr<TimedTask> BuildPrintStringTask(std::string a_str, int a_period, int a_timesToPer = ALWAYS_PERFORM) 
{
    std::shared_ptr<AbstractAct> abstractActPtr = std::make_shared<PrintStrAct>(a_str);
    std::chrono::duration<double> period1(a_period);
    return std::make_shared<TimedTask>(a_str,period1, (abstractActPtr), a_timesToPer);;
}


BEGIN_TEST(basic_3_tasks_run_add_stop)
    std::shared_ptr<TimedTask> TimedTaskB = BuildPrintStringTask("TimedTask B",13,2);
    std::shared_ptr<TimedTask> TimedTaskA = BuildPrintStringTask("TimedTask A", 3,3);
    std::shared_ptr<TimedTask> TimedTaskC = BuildPrintStringTask("TimedTask C",20,1);
    std::shared_ptr<Scheduler> sd1 = std::make_shared<Scheduler>();
    
    std::thread RunThread([&] {
        sd1->Run();
    });
    sd1->AddTimedTask((TimedTaskC));
    sd1->AddTimedTask((TimedTaskA));
    sd1->AddTimedTask((TimedTaskB));

    std::thread StopThread([&] {
        std::this_thread::sleep_for(30s);
        sd1->StopExecution();
    });

    RunThread.join();

    StopThread.join();
	ASSERT_PASS();
END_TEST



BEGIN_TEST(add_new_task_during_run)

    std::shared_ptr<TimedTask> TimedTaskB = BuildPrintStringTask("TimedTask B",13,2);
    std::shared_ptr<TimedTask> TimedTaskA = BuildPrintStringTask("TimedTask A", 3,3);
    std::shared_ptr<TimedTask> TimedTaskC = BuildPrintStringTask("TimedTask C",20,1);
    std::shared_ptr<TimedTask> TimedTaskD = BuildPrintStringTask("TimedTask D",5,1);
    std::shared_ptr<Scheduler> sd1 = std::make_shared<Scheduler>();
    
    std::thread RunThread([&] {
        sd1->Run();
    });
   
    sd1->AddTimedTask((TimedTaskC));
    sd1->AddTimedTask((TimedTaskA));
    sd1->AddTimedTask((TimedTaskB));
    
    std::thread AddLateTaskThread([&] {
        std::this_thread::sleep_for(5s);
        sd1->AddTimedTask((TimedTaskD));
    });


    std::thread StopThread([&] {
        std::this_thread::sleep_for(30s);
        sd1->StopExecution();
    });

    RunThread.join();
    AddLateTaskThread.join();
    StopThread.join();
	ASSERT_PASS();
END_TEST


BEGIN_TEST(stop_while_task_inside)
    std::shared_ptr<TimedTask> TimedTaskB = BuildPrintStringTask("TimedTask B",13,2);
    std::shared_ptr<TimedTask> TimedTaskA = BuildPrintStringTask("TimedTask A", 3,3);
    std::shared_ptr<TimedTask> TimedTaskC = BuildPrintStringTask("TimedTask C",20,1);
    std::shared_ptr<Scheduler> sd1 = std::make_shared<Scheduler>();
    
    std::thread RunThread([&] {
        sd1->Run();
    });
    sd1->AddTimedTask((TimedTaskC));
    sd1->AddTimedTask((TimedTaskA));
    sd1->AddTimedTask((TimedTaskB));

    std::thread StopThread([&] {
        std::this_thread::sleep_for(10s);
        sd1->StopExecution();
    });

    RunThread.join();

    StopThread.join();


	ASSERT_PASS();
END_TEST


BEGIN_TEST(tasks_IDs_check)
    std::shared_ptr<TimedTask> TimedTaskA = BuildPrintStringTask("TimedTask A", 3,3);
    std::shared_ptr<TimedTask> TimedTaskB = BuildPrintStringTask("TimedTask B",13,2);
    std::shared_ptr<TimedTask> TimedTaskC = BuildPrintStringTask("TimedTask C",20,1);
    

    ASSERT_THAT(TimedTaskA->TaskId() < TimedTaskB->TaskId());
    ASSERT_THAT(TimedTaskB->TaskId() < TimedTaskC->TaskId());
    
END_TEST


BEGIN_SUITE(Its what you learn after you know it all that counts)
    TEST(basic_3_tasks_run_add_stop)
    TEST(add_new_task_during_run)
    TEST(stop_while_task_inside)
    TEST(tasks_IDs_check)
END_SUITE
