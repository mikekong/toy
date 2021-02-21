
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

void CostTime() {
  volatile int sum = 0;
  volatile int tmp = 0;
  for (int i = 0; i < 10000; ++i) {
    tmp = i;
    sum += tmp;
  }
}

void TEST(int thread_num, int loop_num) {
  cout << "Start to test thread_num:" << thread_num << " loop_num:" << loop_num << endl;

  {
    vector<unique_ptr<thread>> ts;

    for (int i = 0; i < thread_num; ++i) {
      unique_ptr<thread> t1 = make_unique<thread>([i, loop_num]() {
        auto start = chrono::system_clock::now();

        for (int j = 0; j < loop_num; ++j) {
          CostTime();
        }

        auto end = chrono::system_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        double sec = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
        stringstream ss;

        ss << "thread[" << i << "] Loop[" << loop_num << "] without lock cost: " << sec << endl;

        cout << ss.str();
      });

      ts.push_back(std::move(t1));
    }

    for (size_t i = 0; i < ts.size(); ++i) {
      ts[i]->join();
    }
  }

  std::mutex l;

  {
    vector<unique_ptr<thread>> ts;

    for (int i = 0; i < thread_num; ++i) {
      unique_ptr<thread> t1 = make_unique<thread>([&l, i, loop_num]() {
        auto start = chrono::system_clock::now();

        for (int j = 0; j < loop_num; ++j) {
          std::lock_guard<std::mutex> lg(l);
          CostTime();
        }

        auto end = chrono::system_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        double sec = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
        stringstream ss;

        ss << "thread[" << i << "] Loop[" << loop_num << "] with lock cost: " << sec << endl;

        cout << ss.str();
      });

      ts.push_back(std::move(t1));
    }

    for (size_t i = 0; i < ts.size(); ++i) {
      ts[i]->join();
    }
  }
}

int main(int argc, char ** argv) {
  if (argc < 3) {
    cout << "Invalid params";
    cout << argv[0] << " thread_num loop_num" << endl;
    return 0;
  }

  int thread_num = stoi(argv[1]);
  int loop_num = stoi(argv[2]);

  TEST(1, loop_num);

  TEST(thread_num, loop_num);

  return 0;
}

