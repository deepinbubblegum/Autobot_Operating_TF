#include <iostream>
#include <server.hpp>
#include <stream.hpp>
#include <chrono>
#include <thread>

using namespace std;
int main(int, char**) {
    Server server;
    server.start_step();
    Stream video_stream;
    video_stream.stream_init(server.get_socket());
    this_thread::sleep_for(chrono::seconds(5));
    server.stop_device();

    // Press any key to continue...
    // #ifdef _WIN32
    //     system("pause");
    // #elif __linux__
    //     system("read");
    // #endif
    return 0;
}
