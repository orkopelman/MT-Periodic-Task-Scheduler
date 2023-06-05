#ifndef ABSTRACT_TASK_HPP
#define ABSTRACT_TASK_HPP

#include "actions.hpp"

#include <memory>
#include <iostream>

class AbstractTask {
public:
    virtual ~AbstractTask() = default;
    virtual void Act() = 0;
    
};    

struct AddTask : AbstractTask {
public:
    AddTask(int a, int b) : m_a(a) ,m_b(b) {}
    void Act() override {act::add(m_a,m_b);}

private:
    int m_a;
    int m_b;
};

struct PrintTask : AbstractTask {
public:
    PrintTask(const std::string& a) : m_a(a) {}
    void Act() override {act::print(m_a);}

private:
    std::string m_a;
};

#endif //ABSTRACT_TASK_HPP
