#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

using namespace std;
class Autoloader
{
private:
    string file_config = "./config/config.yaml";

public:
    int Id, Bitrate, Resolution, Emu, GameId;
    string IPaddress, Port;
    bool GPU;
    bool loadUserConfig();
};