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
    cv::Mat src;
    while(true){
        if(!stream.get_img(src))
            break;
        cv::imshow("preview", src);
        int key = cv::waitKey(20);
        if (key == 28)
            break;
    }
    cv::destroyAllWindows();
    server.stop_device();
    return 0;
}
