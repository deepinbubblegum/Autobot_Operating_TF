#include <apps.hpp>

using namespace std;

int main(int argc,char** argv) {
    Autoloader autoload;
    
    YAML::Node UserConfig;


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