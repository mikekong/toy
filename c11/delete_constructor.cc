
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>


using namespace std;

template <class VType>
struct AC {
    AC(VType v) : v(v) {}
    AC(const AC &) = delete;
    AC(const AC &&) = delete;
    AC & operator=(const AC &) = delete;
    AC & operator=(const AC &&) = delete;
    ~AC() {
        cout << "AC: " << v << " destructor" << endl;
    }
    VType v{0};
    mutex m;
};

void vector_f(vector<int> vs) {
    vector<AC<int>> vt;
    for (unsigned i = 0; i < vs.size(); ++i) {
        //  Invalid as vector will use move constructor when resize
        //vt.emplace_back(i);
    }
    if (vs.size() <= 0) return ;

    unique_ptr<AC<int>> varr[vs.size()];

    for (unsigned i = 0; i < vs.size(); ++i) {
        varr[i] = make_unique<AC<int>>(i); 
    }
}

void hash_f(vector<string> vs) {
    unordered_map<string, unique_ptr<AC<string>>> name2inst;

    for (auto & n : vs) {
        name2inst[n] = make_unique<AC<string>>(n);
    }

    for (auto & it : name2inst) {
        cout << it.first << "->" << it.second->v << endl;
    }
}

void rbtree_f(vector<string> vs) {
    map<string, unique_ptr<AC<string>>> name2inst;

    for (auto & n : vs) {
        name2inst[n] = make_unique<AC<string>>(n);
    }

    for (auto & it : name2inst) {
        cout << it.first << "->" << it.second->v << endl;
    }
}

int main() {
    AC<int> a(2);
    vector_f(vector<int>{1,2,3,4});
    hash_f(vector<string>{"a1", "b22", "c333", "d4444", "e55555"});
    rbtree_f(vector<string>{"e55555", "b22", "c333", "d4444", "a1"});
    return 0;
}
