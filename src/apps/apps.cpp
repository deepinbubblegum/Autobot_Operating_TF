#include <iostream>
#include <chrono>
#include <thread>
#include <server.hpp>
#include <stream.hpp>
#include <controller.hpp>
#include <autoloader.hpp>

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

int main(int argc,char** argv) {
    Autoloader autoload;
    autoload.loadUserConfig();
    // load confi
    int deviceID = autoload.Id;
    string deviceIp = autoload.IPaddress;
    string devicePort = autoload.Port; 
    int bitrate = autoload.Bitrate;
    int resolution = autoload.Resolution;

    Server server;
    server.setDevice(
        deviceID, deviceIp, 
        devicePort, bitrate, 
        resolution
    ); // setup Devices server
    server.start_step(); // start server
    vector<string> device_list = server.getDevice_list();
    
    string deviceName = device_list[deviceID];
    cout << "device target name :" << deviceName << endl;
    Controller ctl;
    ctl.exec();

    server.stop_device();
    return 0;
}