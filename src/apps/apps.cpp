#include <iostream>
#include <server.hpp>
#include <adb.hpp>

using namespace std;
int main(int, char**) {
    adb adb_;
    adb_.start();

    // Press any key to continue...
    // #ifdef _WIN32
    //     system("pause");
    // #elif __linux__
    //     system("read");
    // #endif
}
