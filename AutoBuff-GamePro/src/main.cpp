#include <iostream>

#ifndef CMAKEMACROSAMPLE
    #define CMAKEMACROSAMPLE "NO SYSTEM NAME"
#endif

using namespace std;
int main() {
    
    cout << "Hello CMake!" << endl;
    cout << CMAKEMACROSAMPLE << endl;
    return 0;
}
