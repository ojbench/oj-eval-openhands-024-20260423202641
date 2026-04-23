#include <iostream>
#include "deque.hpp"

int main() {
    sjtu::deque<int> dq1;
    
    for (int i = 0; i < 100; ++i) {
        dq1.push_back(i);
    }
    
    std::cout << "Original size: " << dq1.size() << std::endl;
    
    std::cout << "Creating copy..." << std::endl;
    sjtu::deque<int> dq2(dq1);
    
    std::cout << "Copy size: " << dq2.size() << std::endl;
    
    std::cout << "Checking copy..." << std::endl;
    for (size_t i = 0; i < dq2.size(); ++i) {
        if (dq2[i] != (int)i) {
            std::cout << "ERROR at " << i << ": expected " << i << ", got " << dq2[i] << std::endl;
            return 1;
        }
    }
    
    std::cout << "Copy correct!" << std::endl;
    
    std::cout << "Modifying original..." << std::endl;
    dq1.push_back(999);
    
    std::cout << "Original size: " << dq1.size() << std::endl;
    std::cout << "Copy size: " << dq2.size() << std::endl;
    
    std::cout << "Test passed!" << std::endl;
    return 0;
}
