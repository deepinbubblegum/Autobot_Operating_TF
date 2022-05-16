#include <server.hpp>

bool Server::get_device_name()
{
    if (s_adbPath.empty())
    {
        exec_result = run_exec("adb devices");
        regex devices_name_rule("[^\r\n]+\\s");
        cmatch m;
        vector<string> list;
        while (regex_search(exec_result.c_str(), m, devices_name_rule))
        {
            list.push_back(m.str());
            exec_result = m.suffix().first;
        }
        for (int i = 1; i < list.size(); i++)
        {
            stringstream ss(list[i]);
            string device_name;
            ss >> device_name;
            device_name_list.push_back(device_name);
        }
        if (!device_name_list.empty())
        {
            cout << "######## Devices list ########" << endl;
            for (string device_name : device_name_list)
                cout << device_name << endl;
            return true;
        }
    }
    return false;
}

string Server::run_exec(string origin_cmd)
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

bool Server::push_server_to_device(string server_path)
{
    string cmd = "adb -s " + device_name_list[index_device] + " push " + server_path + " " + server_device_path;
    cout << cmd << endl;
    string result = run_exec(cmd);
    regex result_rule("pushed");
    cmatch m;
    if (!regex_search(result.c_str(), m, result_rule))
    {
        return false;
    }
    // cout << m.str() << endl; //pushed
    return true;
}

bool Server::reverse_config(string domain_socket_name, string local_port)
{
    string cmd = "adb -s " + device_name_list[index_device] + " reverse " + domain_socket_name + " " + local_port;
    cout << cmd << endl;
    system(cmd.c_str());
    string result = run_exec("adb -s " + device_name_list[index_device] + " reverse --list");
    cout << result << endl;
    regex result_rule(local_port);
    cmatch m;
    if (!regex_search(result.c_str(), m, result_rule))
    {
        return false;
    }
    // cout << m.str() << endl; // tcp:[port]
    return true;
}

bool Server::video_socket_init(string id, string port)
{
    assert(!(id.empty() || port.empty()));
    // winsock
    cout << "Initialising Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        cout << "Failed. Error Code : " << WSAGetLastError() << endl;
        return false;
    }
    cout << "Initialised." << endl;
    _socket_fd = socket(PF_INET, SOCK_STREAM, 0);
    if ((_socket_fd < 0))
    {
        cout << "Could not create socket : " << WSAGetLastError() << endl;
        return false;
    }
    cout << "Socket created." << endl;

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(port.c_str()));
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    char opt = 1;
    setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(_socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cout << "bind failed" << endl;
        return false;
    }

    if (listen(_socket_fd, 10) < 0)
    {
        cout << "Monitoring failed" << endl;
        return false;
    }
    struct sockaddr_in client_addr;
    socklen_t client_addrlength = sizeof(client_addr);
    _video_socket = accept(_socket_fd, (struct sockaddr *)&client_addr, &client_addrlength); // connect socket
    if (_video_socket < 0)
    {
        cout << "Socket get failed" << endl;
        return false;
    }
    cout << "Video socket initialized successfully" << endl
         << "The connection client id is:" << inet_ntoa(client_addr.sin_addr) << endl;
    return true;
}

bool Server::setDevice(int device_id, string ip_address, string port, int bitrate, int resolution)
{
    index_device = device_id;
    string _ip = ip_address; // default ip loop back
    _port = port;
    int _bitrate = bitrate; // default 200000000
    int _resolution = resolution;
    return true;
}

bool Server::start_run_in_device()
{
    string cmd = "adb -s " + device_name_list[index_device] + " shell CLASSPATH=/data/local/tmp/scrcpy-server app_process \
    / com.genymobile.scrcpy.Server 1.14 info " +
                 to_string(_resolution) + " " + to_string(_bitrate) +
                 " 120 -1 false - true true 0 false false profile=1,level=1" + " > temp.txt";
    auto tmp = [](string cmd_use)
    {
        system(cmd_use.c_str());
    };
    _device_server_thread = thread(tmp, cmd);
    // string id = _ip;
    // string port = _port;
    if (!video_socket_init(_ip, _port))
    {
        return false;
    }
    ifstream terminal_out("temp.txt");
    string terminal_result;
    string tmps;
    while (getline(terminal_out, tmps))
    {
        terminal_result += "\n" + tmps;
    }
    system("del /s temp.txt");
    cout << terminal_result << endl;
    regex result_rule("ERROR");
    cmatch m;
    if (regex_search(terminal_result.c_str(), m, result_rule))
    {
        return false;
    }
    return true;
}

bool Server::stop_device()
{
    string cmd = "adb -s " + device_name_list[index_device] + " shell ps | findstr app_process";
    cout << cmd << endl;
    string t_out = run_exec(cmd);
    regex pid_rule("[0-9]+");
    cmatch m;
    vector<string> pid_list;
    regex_search(t_out.c_str(), m, pid_rule);
    string pid = m.str();
    cmd = "adb -s " + device_name_list[index_device] + " shell kill -9 " + pid;
    system(cmd.c_str());
    shutdown(_video_socket, SD_BOTH);
    shutdown(_socket_fd, SD_BOTH);
    // adb_kill();
    return true;
}

bool Server::adb_kill()
{
    system("adb kill-server");
    return true;
}

bool Server::remove_server_from_device(string server_path)
{
    string cmd = "adb -s " + device_name_list[index_device] + " shell rm -rf " + server_path;
    system(cmd.c_str());
    return true;
}

bool Server::remove_reverse(string domain)
{
    string cmd = "adb -s " + device_name_list[index_device] + " reverse --remove domain";
    system(cmd.c_str());
    return true;
}

bool Server::aftermath()
{
    bool ret = false;
    switch (_server_stat)
    {
    case INIT_STAT:
        break;
    case PUSH_SUCCESS:
    {
        remove_server_from_device("/data/local/tmp/scrcpy-server");
        break;
    }
    case REVERSE_SUCCESS:
    {
        remove_server_from_device("/data/local/tmp/scrcpy-server");
        remove_reverse("localabstract:scrcpy");
        break;
    }
    case RUN_SUCCESS:
    {
        /*
        remove_server_from_device("/data/local/tmp/scrcpy-server");
        remove_reverse("localabstract:scrcpy");
        */
        ret = true;
        break;
    }
    }
    return ret;
}

bool Server::start_step()
{
    switch (_server_stat)
    {
    case INIT_STAT:
    {
        if (!get_device_name())
            break;
        if (!push_server_to_device(server_path_file))
            break;
        _server_stat = PUSH_SUCCESS;
    }
    case PUSH_SUCCESS:
    {
        if (!reverse_config("localabstract:scrcpy", "tcp:" + _port))
            break;
        _server_stat = REVERSE_SUCCESS;
    }
    case REVERSE_SUCCESS:
    {
        if (!start_run_in_device())
            break;
        _server_stat = RUN_SUCCESS;
    }
    }
    return aftermath();
}

int Server::get_socket()
{
    return _video_socket;
}

vector<string> Server::getDevice_list()
{
    return device_name_list;
}