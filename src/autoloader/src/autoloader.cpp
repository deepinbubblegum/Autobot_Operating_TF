#include <autoloader.hpp>

bool Autoloader::loadConfig(){
    if(!loadUserConfig()){
        return false;
    }
    if(!loadPrefix()){
        return false;
    }
    return true;
}

bool Autoloader::loadUserConfig(){
    YAML::Node config;
    try{
        config = YAML::LoadFile(file_config);
    }catch(YAML::BadFile &e){
        std::cout<<"load config error!"<<std::endl;
        return false;
    }

    Id = config["UserConfig"]["Id"].as<int>();
    IPaddress = config["UserConfig"]["IPaddress"].as<string>();
    Port = config["UserConfig"]["Port"].as<string>();
    Bitrate = config["UserConfig"]["Bitrate"].as<int>();
    Resolution = config["UserConfig"]["Resolution"].as<int>();
    Emu = config["UserConfig"]["Emu"].as<int>();
    GameId = config["UserConfig"]["GameId"].as<int>();
    GPU = config["UserConfig"]["GPU"].as<bool>();
    return true;
}

bool Autoloader::loadPrefix(){
    YAML::Node prefix;
    try{
        prefix = YAML::LoadFile(file_prefix);
    }catch(YAML::BadFile &e){
        std::cout<<"load prefix error!"<<std::endl;
        return false;
    }

    switch (Emu)
    {
    case BlueStacks:
        devInput = prefix["touch_input"][0].as<string>();
        break;
    case LDPlayer:
        devInput = prefix["touch_input"][1].as<string>();
        break;
    }
    cout << devInput << endl;
    return true;
}