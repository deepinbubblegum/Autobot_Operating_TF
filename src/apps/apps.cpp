#include <iostream>
#include <server.hpp>
#include <stream.hpp>
#include <controller.hpp>
#include <chrono>
#include <thread>

using namespace std;
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

int main(int, char**) {
    Server server;
    int deviceID = 0;
    string devicePort = "2000";
    server.setDevice(deviceID, devicePort); // device id and port server
    server.start_step();
    vector<string> device_list = server.getDevice_list();
    
    string deviceName = device_list[deviceID];
    cout << "device target name :" << deviceName << endl;
    Controller ctl;
    ctl.exec();

    server.stop_device();
    return 0;
}