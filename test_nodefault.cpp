#include <iostream>
#include "deque.hpp"

class NoDefault {
    int value;
public:
    NoDefault(int v) : value(v) {}
    NoDefault(const NoDefault& other) : value(other.value) {}
    int getValue() const { return value; }
};

int main() {
    sjtu::deque<NoDefault> dq;
    
    std::cout << "Pushing elements..." << std::endl;
    for (int i = 0; i < 10; ++i) {
        dq.push_back(NoDefault(i));
    }
    
    std::cout << "Size: " << dq.size() << std::endl;
    
    std::cout << "Accessing elements..." << std::endl;
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << "dq[" << i << "] = " << dq[i].getValue() << std::endl;
    }
    
    std::cout << "Test passed!" << std::endl;
    return 0;
}
