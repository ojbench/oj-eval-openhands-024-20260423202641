#include <iostream>
#include <string>
#include "deque.hpp"

using namespace std;

int main() {
    int n;
    cin >> n;
    
    sjtu::deque<int> dq;
    
    for (int i = 0; i < n; ++i) {
        string op;
        cin >> op;
        
        if (op == "push_back") {
            int val;
            cin >> val;
            dq.push_back(val);
        } else if (op == "push_front") {
            int val;
            cin >> val;
            dq.push_front(val);
        } else if (op == "pop_back") {
            dq.pop_back();
        } else if (op == "pop_front") {
            dq.pop_front();
        } else if (op == "size") {
            cout << dq.size() << endl;
        } else if (op == "front") {
            cout << dq.front() << endl;
        } else if (op == "back") {
            cout << dq.back() << endl;
        } else if (op == "at") {
            int idx;
            cin >> idx;
            cout << dq.at(idx) << endl;
        } else if (op == "empty") {
            cout << (dq.empty() ? "true" : "false") << endl;
        } else if (op == "clear") {
            dq.clear();
        }
    }
    
    return 0;
}
