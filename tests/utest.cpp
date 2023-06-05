#include "mu_test.h"
#include "schduler.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

BEGIN_TEST(first)
    std::unique_ptr<AbstractTask> ptrAbsTask1 = std::make_unique<PrintTask>("task A");
    std::unique_ptr<AbstractTask> ptrAbsTask2 = std::make_unique<PrintTask>("task B");
    std::unique_ptr<AbstractTask> ptrAbsTask3 = std::make_unique<PrintTask>("task C");

    std::unique_ptr<Task> taskA = std::make_unique<Task>(10,std::move(ptrAbsTask1),3);
    std::unique_ptr<Task> taskB = std::make_unique<Task>(13,std::move(ptrAbsTask2));
    std::unique_ptr<Task> taskC = std::make_unique<Task>(5,std::move(ptrAbsTask3),4);

    std::unique_ptr<Schduler> sd1 = std::make_unique<Schduler>();

    sd1->AddTask(std::move(taskA));
    sd1->AddTask(std::move(taskB));
    sd1->AddTask(std::move(taskC));

    sd1->Run();


 
	ASSERT_PASS();
END_TEST



BEGIN_TEST(sec)

    
    std::unique_ptr<AbstractTask> ptrAbsTask1 = std::make_unique<PrintTask>("task A");
    std::unique_ptr<AbstractTask> ptrAbsTask2 = std::make_unique<PrintTask>("task B");
    std::unique_ptr<AbstractTask> ptrAbsTask3 = std::make_unique<PrintTask>("task C");

    std::unique_ptr<Task> taskA = std::make_unique<Task>(10,std::move(ptrAbsTask1),3);
    std::unique_ptr<Task> taskB = std::make_unique<Task>(13,std::move(ptrAbsTask2));
    std::unique_ptr<Task> taskC = std::make_unique<Task>(5,std::move(ptrAbsTask3),4);

    std::unique_ptr<Schduler> sd1 = std::make_unique<Schduler>();


    std::thread AddThread([&]() {
        sd1->AddTask(std::move(taskA));
        sd1->AddTask(std::move(taskB));
        sd1->AddTask(std::move(taskC));
    });

    std::thread RunThread([&]() {
        std::this_thread::sleep_for(2s);
        sd1->Run();
    });
    

    // std::thread pauseThread([&]() {
    //     std::this_thread::sleep_for(20s);
    //     int tasksToPerform = sd1->PauseExcecution();
    //     std::cout << "excution was paused, there are " << tasksToPerform << " Tasks to perofrm \n";
    //     //sd1->Run();

    // });

    // std::thread resumeThread([&] {
    //     std::this_thread::sleep_for(25s);
    //     sd1->Run();
        
    // });

    AddThread.join();
    RunThread.join();
    //pauseThread.join();
    //resumeThread.join();
 
	ASSERT_PASS();
END_TEST




BEGIN_SUITE(Its what you learn after you know it all that counts)
	TEST(sec)

END_SUITE
