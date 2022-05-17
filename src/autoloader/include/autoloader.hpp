#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

using namespace std;
class Autoloader
{
private:
    enum EmulatorID{
        BlueStacks,     //Blustacks
        LDPlayer        //native resolution
    };

    string file_config = "./config/config.yaml";
    string file_prefix = "./prefix/prefix.yaml";
    bool loadUserConfig();
    bool loadPrefix();

public:
    int Id, Bitrate, Resolution, Emu, GameId;
    string IPaddress, Port, devInput;
    bool GPU;
    bool loadConfig();
};