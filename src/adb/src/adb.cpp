#include <adb.hpp>

bool adb::get_device_name(){
    if (s_adbPath.empty()){
        exec_result = run_exec("adb devices");
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

string adb::run_exec(string origin_cmd) {
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
    cout << cmd << endl;
    string result = run_exec(cmd);
    regex result_rule("pushed");
    cmatch m;
    if(!regex_search(result.c_str(), m, result_rule)){
        return false;
    }
    // cout << m.str() << endl; //pushed
    return true;
}

bool adb::reverse_config(string domain_socket_name, string local_port){
    string cmd ="adb -s " + device_name_list[index_device] + " reverse "  
        + domain_socket_name + " " + local_port;
    cout << cmd << endl;
    system(cmd.c_str());
    string result = run_exec("adb -s " + device_name_list[index_device] + " reverse --list");
    cout << result << endl;
    regex result_rule(local_port);
    cmatch m;
    if(!regex_search(result.c_str(), m, result_rule)){
        return false;
    }
    // cout << m.str() << endl; // tcp:[port]
    return true;
}

bool adb::start_device(){
    string cmd = "adb -s "+ device_name_list[index_device] 
    + " shell CLASSPATH=/data/local/tmp/scrcpy-server app_process \
    / com.genymobile.scrcpy.Server 1.14 info " + to_string(_resolution) + " " + to_string(_bitrate) +
    " 120 -1 false - true true 0 false false profile=1,level=1" + " > temp.txt";
    auto tmp = [](string cmd_use){
        system(cmd_use.c_str());
    };
    _device_server_thread = thread(tmp, cmd);
    
    // ยังไม่เสร็จ
    return true;
}

bool adb::stop_device(){
    string cmd = "adb -s "+ device_name_list[index_device]  +  " shell ps | grep app_process";
    cout << cmd << endl;
    string t_out = run_exec(cmd);
}

int adb::get_socket(){
    return _socket;
}

bool adb::start(){
    get_device_name();
    push_server_to_device(server_path_file);
    reverse_config("localabstract:scrcpy","tcp:" + _port);
    return true;
}