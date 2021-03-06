#include <iostream>
#include <chrono>
#include <string>
#include <thread>
#include <deque>
#include <atomic>
#include "util/mutexlock.h"

using namespace std;

//  TestSafeClass 
class TestSafeClass {
 public:
  using Timestamp = std::chrono::time_point<std::chrono::system_clock>;
  //  default window size is 100000
  TestSafeClass() = default;
  ~TestSafeClass() = default;

  void OnRecv() {
    ++recv_cnt_;
    auto now = std::chrono::system_clock::now();
    MutexLock _l(&mutex_); 
    if (costs_.empty()) {
      costs_.push_back(std::chrono::nanoseconds{0});
    } else {
      auto diff = now - last_recv_ts_;
      costs_.push_back(diff);
    }
    last_recv_ts_ = now;
  }

  void Output() {
    MutexLock _l(&mutex_);
    if (costs_.size() <= 0) {
      return;
    }

    auto min_diff = costs_.front();
    auto max_diff = min_diff;
    decltype(min_diff) sum{0};
    for (auto& cost_nano : costs_) {
      if (cost_nano < min_diff) {
        min_diff = cost_nano;
      }
      if (cost_nano > max_diff) {
        max_diff = cost_nano;
      }
      sum += cost_nano;
    }
  }

  static int64_t recv_cnt() {
    return recv_cnt_.load();
  }

 private:
  Mutex mutex_;
  std::deque<std::chrono::nanoseconds> costs_ GUARDED_BY(mutex_);
  Timestamp last_recv_ts_ GUARDED_BY(mutex_);
  static std::atomic<int64_t> recv_cnt_;
};

int main(int argc, char ** argv) {
  return 0;
}


