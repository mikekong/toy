
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>


using namespace std;

struct AC {
    AC(int v) : v(v) {}
    AC(const AC &) = delete;
    AC(const AC &&) = delete;
    AC & operator=(const AC &) = delete;
    AC & operator=(const AC &&) = delete;
    ~AC() {
        cout << "AC: " << v << " destructor" << endl;
    }
    int v{0};
    mutex m;
};

void f1(vector<int> vs) {
    vector<AC> vt;
    for (unsigned i = 0; i < vs.size(); ++i) {
        //vt.emplace_back(i);
    }
    if (vs.size() <= 0) return ;

    unique_ptr<AC> varr[vs.size()];

    for (unsigned i = 0; i < vs.size(); ++i) {
        varr[i] = make_unique<AC>(i); 
    }
}


int main() {
    AC a(2);
    f1(vector<int>{1,2,3,4});
    return 0;
}
