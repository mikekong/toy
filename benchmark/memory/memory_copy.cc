
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace chrono;

int main(int argc, char ** argv) {
  if (argc < 3) {
    cout << "Error args" << endl;
    cout << argv[0] << " thread_num memory_size loop_cnt" << endl;
    return 0;
  };

  vector<unique_ptr<thread>> threads;

  int thread_num = stoi(argv[1]);
  int memory_size = stoi(argv[2]);
  int max_loop_cnt = stoi(argv[3]);

  for (int i = 0; i < thread_num; ++i) {
    unique_ptr<thread> t = make_unique<thread>([i, memory_size, max_loop_cnt] {
      char * src_ptr = new char[memory_size];
      char * dst_ptr = new char[memory_size];

      auto start = chrono::system_clock::now();
      for (int j = 0; j < max_loop_cnt; ++j) {
        memcpy(dst_ptr, src_ptr, memory_size);
      }
      auto end = chrono::system_clock::now();
      auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
      double sec = double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den;
      stringstream ss;
      ss << "Hello thread:" << i << " Copy Over, cost:" << sec << endl;
      cout << ss.str();

      delete [] src_ptr;
      delete [] dst_ptr;
    });

    threads.push_back(std::move(t));
  }

  for (int i = 0; i < thread_num; ++i) {
    threads[i]->join();
  }

  return 0;
}

