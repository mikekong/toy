
#include <iostream>
#include <numeric>

using namespace std;

int fun(int n) {
  long long arr[n];
  cout << n << endl;

  long long sum = 0;
  for (int i = 0; i < n; ++i) {
    arr[i] = i;
    sum += arr[i];
  }

  return sum;
}

int fun2() {
  int n = 20;

  cout << "fun2" << endl;

  long long arr[n];

  long long sum = 0;
  for (int i = 0; i < n; ++i) {
    arr[i] = i;
    sum += arr[i];
  }

  return sum;
}

int main() {
  fun2();
  fun(1);
  fun(10);
  fun(0);
  //fun(std::numeric_limits<uint32_t>::max());
  for (int i = 1; ; i *= 10) {
    fun(i);
  }
  fun(-1);
  return 0;
}
