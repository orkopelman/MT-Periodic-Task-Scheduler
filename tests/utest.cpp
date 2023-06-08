#include "mu_test.h"
#include "scheduler.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

std::unique_ptr<TimedTask> BuildPrintStringTask(std::string a_str, int a_period, int a_timesToPer = ALWAYS_PERFORM) 
{
    std::unique_ptr<AbstractTask> ptrAbsTimedTask1 = std::make_unique<PrintTask>(a_str);
    std::chrono::duration<double> period1(a_period);
    return std::make_unique<TimedTask>(period1,std::move(ptrAbsTimedTask1),a_timesToPer);
}

BEGIN_TEST(first)
    std::unique_ptr<TimedTask> TimedTaskA = BuildPrintStringTask("TimedTask A",10,3);
    std::unique_ptr<TimedTask> TimedTaskB = BuildPrintStringTask("TimedTask B",13);
    std::unique_ptr<TimedTask> TimedTaskC = BuildPrintStringTask("TimedTask C",5,4);

    std::unique_ptr<Schduler> sd1 = std::make_unique<Schduler>();

    sd1->AddTimedTask(std::move(TimedTaskA));
    sd1->AddTimedTask(std::move(TimedTaskB));
    sd1->AddTimedTask(std::move(TimedTaskC));

    sd1->Run();

	ASSERT_PASS();
END_TEST



BEGIN_TEST(sec)

    std::unique_ptr<TimedTask> TimedTaskA = BuildPrintStringTask("Task A Exceuted",10,3);
    std::unique_ptr<TimedTask> TimedTaskB = BuildPrintStringTask("Task B Exceuted",13);
    std::unique_ptr<TimedTask> TimedTaskC = BuildPrintStringTask("Task C Exceuted",5,4);

    std::unique_ptr<Schduler> sd1 = std::make_unique<Schduler>();

    std::thread AddThread([&]() {
        sd1->AddTimedTask(std::move(TimedTaskA));
        sd1->AddTimedTask(std::move(TimedTaskB));
    });

    std::thread RunThread([&]() {
        std::this_thread::sleep_for(3s);
        sd1->Run();
    });
    std::this_thread::sleep_for(3s);
    
    sd1->AddTimedTask(std::move(TimedTaskC));

    std::thread pauseThread([&]() {
        std::this_thread::sleep_for(20s);
        int TimedTasksToPerform = sd1->PauseExcecution();
        std::cout << "exceution was paused, there are " << TimedTasksToPerform << " TimedTasks to perform \n";
        sd1->ResumeExcecution();

    });


    AddThread.join();
    RunThread.join();
    pauseThread.join();
    
 
	ASSERT_PASS();
END_TEST




BEGIN_SUITE(Its what you learn after you know it all that counts)
	TEST(sec)

END_SUITE
