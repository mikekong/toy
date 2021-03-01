
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <iostream>

using namespace std;

int main(int argc, char ** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true); 
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "gLog with Info level" << endl;

    cout << "Exit" << endl;

    google::ShutdownGoogleLogging(); 

    return 0;
}

