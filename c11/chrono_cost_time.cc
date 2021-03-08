#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

using namespace std;

void f() {
  auto start_ts = chrono::system_clock::now();

  const static int LOOP = 100000000;
  for (int i = 0; i < LOOP; ++i) {
    auto curr = chrono::system_clock::now();
  }

  auto stop_ts  = chrono::system_clock::now();
  printf("Loop[%d] Cost %ld ns Per[%ld]\n"
      , LOOP, (stop_ts - start_ts).count()
      , ((stop_ts - start_ts).count()) / LOOP);
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    std::cout << argv[0] << " thread_num " << std::endl;
    return 0;
  }

  vector<unique_ptr<thread>> threads;

  for (int i = 0; i < stoi(argv[1]); ++i) {
    threads.emplace_back(new thread(f));
  }

  for (auto & thread : threads) {
    thread->join();
  }
}

