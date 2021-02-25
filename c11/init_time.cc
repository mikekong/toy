#include <iostream>

using namespace std;

int FLAGS_argv;

class D {
private:
  int v = 0;
  static D ZERO;
public:
  D(int v) : v(v) {
    cout << "Constructor:" << v << endl;
  }

  ~D() {
    cout << "Destructor:" << v << endl;
  }
};

D D::ZERO = D(0);
static D d1(1);
const static D d2(2);

void f1() {
  static D d3(3);

  D d4(4);
}

void f2() {
  const static D d5(5);
}

int r999(){
  return FLAGS_argv;
}

static D d(r999()); 

int main() {
  FLAGS_argv = 999;
  cout << "main now..." << endl;
  f1();

  D nd(r999());
  return 0;
}

