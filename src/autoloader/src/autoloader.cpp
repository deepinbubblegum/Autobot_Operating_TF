#include <autoloader.hpp>

bool Autoloader::loadUserConfig(){
    YAML::Node config;
    try{
        config = YAML::LoadFile(file_config);
    }catch(YAML::BadFile &e){
        std::cout<<"load config error!"<<std::endl;
        return false;
    }

    Id = config["UserConfig"]["Id"].as<int>();
    Port = config["UserConfig"]["Port"].as<string>();
    Emu = config["UserConfig"]["Emu"].as<int>();
    GameId = config["UserConfig"]["GameId"].as<int>();
    GPU = config["UserConfig"]["GPU"].as<bool>();
    return true;
}