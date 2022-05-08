#include <iostream>
#include <server.hpp>

using namespace std;
int main(int, char**) {
    server serv_;
    serv_.say_hello();
    cout << "Hello, world!\n";
}
