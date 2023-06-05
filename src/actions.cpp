#include "actions.hpp"

namespace act {

int add(int a, int b) {
    
    return a + b;
}

int print(const std::string& a_str) {
    std::cout << a_str << "\n";
    return 1;
}

int calcAvgOf3(int a, int b, int c) {
    return (a+b+c) / 3;
}

}