#include <iostream>
#include <server.hpp>
#include <stream.hpp>
#include <chrono>
#include <thread>

using namespace std;
int main(int, char**) {
    Server server;
    server.start_step();
    Stream stream;
    stream.stream_init(server.get_socket());
    // this_thread::sleep_for(chrono::seconds(5));
    cv::Mat src;
    while(true){
        if(!stream.get_img(src))
            break;
        cv::imshow("origin", src);
        int key = cv::waitKey(1);
        if (key == 28)
            break;
    }
    server.stop_device();

    // Press any key to continue...
    // #ifdef _WIN32
    //     system("pause");
    // #elif __linux__
    //     system("read");
    // #endif
    return 0;
}
