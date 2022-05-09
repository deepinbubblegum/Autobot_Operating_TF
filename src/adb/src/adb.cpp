#include <adb.hpp>

bool adb::get_device_name(){
    if (s_adbPath.empty()){
        exec_result = exec("adb devices");
        regex devices_name_rule("[^\r\n]+\\s");
        cmatch m;
        vector<string> list;
        while(regex_search(exec_result.c_str(), m, devices_name_rule)){
            list.push_back(m.str());
            exec_result = m.suffix().first;
        }
        for(int i = 1; i < list.size(); i++) {
            stringstream ss(list[i]);
            string device_name;
            ss >> device_name;
            device_name_list.push_back(device_name);
        }
        if (!device_name_list.empty()){
            cout << "######## Devices list ########" << endl;
            for(string device_name : device_name_list)
                cout << device_name << endl;
            return true;
        }
    }
    return false;
}

string adb::exec(string origin_cmd) {
    const char* char_cmd = origin_cmd.c_str();
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(char_cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

bool adb::push_server_to_device(string server_path){
    string cmd = "adb -s " + device_name_list[index_device] + " push " + server_path + " " + server_device_path;
    string result = exec(cmd);
    regex result_rule("pushed");
    cmatch m;
    if(!regex_search(result.c_str(), m, result_rule)){
        return false;
    }
    return true;
}