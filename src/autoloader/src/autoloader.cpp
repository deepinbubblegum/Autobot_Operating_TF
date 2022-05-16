#include <autoloader.hpp>

YAML::Node Autoloader::getUserConfig(){
    YAML::Node config;
    config = YAML::LoadFile(file_config);
    return config["UserConfig"];
}