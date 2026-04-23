#include <iostream>
#include "deque.hpp"

int main() {
    sjtu::deque<int> dq;
    
    std::cout << "Pushing 5 elements..." << std::endl;
    for (int i = 0; i < 5; ++i) {
        dq.push_back(i);
    }
    
    std::cout << "Before insert: ";
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Inserting 99 at position 2..." << std::endl;
    auto it = dq.begin();
    it += 2;
    dq.insert(it, 99);
    
    std::cout << "After insert: ";
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Erasing element at position 2..." << std::endl;
    it = dq.begin();
    it += 2;
    dq.erase(it);
    
    std::cout << "After erase: ";
    for (size_t i = 0; i < dq.size(); ++i) {
        std::cout << dq[i] << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Test passed!" << std::endl;
    return 0;
}
