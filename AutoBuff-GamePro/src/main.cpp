#include <iostream>
#include <server.hpp>

#ifndef CMAKEMACROSAMPLE
    #define CMAKEMACROSAMPLE "NO SYSTEM NAME"
#endif

using namespace std;
int main() {
    server serv_; // new object
    serv_.sayhello();
    cout << "Hello CMake!" << endl;
    cout << CMAKEMACROSAMPLE << endl;
    return 0;
}

