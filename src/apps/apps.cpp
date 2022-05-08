#include <iostream>
#include <server.hpp>

using namespace std;
int main(int, char**) {
    server serv_;
    serv_.say_hello();
    cout << "Hello, world!\n";
    

    // Press any key to continue...
    #ifdef _WIN32
        system("pause");
    #elif __linux__
        system("read");
    #endif
}
