#include <gflags/gflags.h>
#include <iostream>

using namespace std;

DEFINE_bool(male, true, "Is Male");
DEFINE_int32(age, 20, "Age");
DEFINE_string(name, "NAME", "Your name");


int main(int argc, char ** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true); 

    std::cout << "Male:" << FLAGS_male << std::endl;
    std::cout << "Age:" << FLAGS_age << std::endl;
    std::cout << "Name:" << FLAGS_name << std::endl;

    return 0;
}

