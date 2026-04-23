#include <iostream>
#include "deque.hpp"

int main() {
    sjtu::deque<int> dq;
    
    std::cout << "Pushing 1000 elements..." << std::endl;
    for (int i = 0; i < 1000; ++i) {
        dq.push_back(i);
    }
    
    std::cout << "Size: " << dq.size() << std::endl;
    
    std::cout << "Checking elements..." << std::endl;
    for (size_t i = 0; i < dq.size(); ++i) {
        if (dq[i] != (int)i) {
            std::cout << "ERROR at " << i << ": expected " << i << ", got " << dq[i] << std::endl;
            return 1;
        }
    }
    
    std::cout << "All elements correct!" << std::endl;
    
    std::cout << "Testing push_front..." << std::endl;
    for (int i = 0; i < 100; ++i) {
        dq.push_front(-i-1);
    }
    
    std::cout << "Size after push_front: " << dq.size() << std::endl;
    
    std::cout << "Test passed!" << std::endl;
    return 0;
}
