#include <iostream>
#include <chrono>
#include <thread>
#include <server.hpp>
#include <stream.hpp>
#include <controller.hpp>
#include <autoloader.hpp>

class Apps
{
private:

public:

};

// int main(int, char**) {
//     Server server;
//     server.setDevice(0, "2000"); // device id and port server
//     server.start_step();
//     Stream stream;
//     stream.stream_init(server.get_socket());
//     cv::Mat src;
//     while(true){
//         if(!stream.get_img(src))
//             break;
//         cv::imshow("preview", src);
//         char key = cv::waitKey(30);
//         if(key == 27)
//             break;
//     }
//     cv::destroyAllWindows();
//     server.stop_device();
//     return 0;
// }