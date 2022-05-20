#include <controller.hpp>

string Controller::findDeviceInput(string device){
    return
        "adb -s "+ device +" shell getevent -pl";
}


void Controller::test(string device){
    exec_result = run_exec(findDeviceInput(device));
    cout << exec_result << endl;
}

// example data
// /dev/input/event[number]: 0003 x position 
// /dev/input/event4: 0003 y position

// Set coordinates

bool Controller::setCoordinates(){
    return true;
}

bool Controller::touch(){
    return true;
}

string Controller::run_exec(string origin_cmd)
{
    const char *char_cmd = origin_cmd.c_str();
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(char_cmd, "r");
    if (!pipe)
        throw std::runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}