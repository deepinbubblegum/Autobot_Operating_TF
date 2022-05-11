#include <iostream>
#include <server.hpp>

using namespace std;
int main(int, char**) {
    server _server;
    _server.start_step();
    cout << _server.get_socket() << endl;
    // _server.stop_device();

    // Press any key to continue...
    // #ifdef _WIN32
    //     system("pause");
    // #elif __linux__
    //     system("read");
    // #endif
}
