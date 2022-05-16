#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

using namespace std;
class Autoloader
{
private:
    string file_config = "./config/config.yaml";

public:
    YAML::Node getUserConfig();
};