#include <iostream>
#include <regex>
#include <bits/stdc++.h>
#include <thread>

using namespace std;
class adb
{
private:
    // variable
    string s_adbPath;
    string exec_result;
    vector<string> device_name_list;
    int index_device = 1;

    // path scrcpy
    string server_path_file = "scrcpy-server/scrcpy-server";
    string server_device_path = "/data/local/tmp/scrcpy-server";

    string _ip ="127.0.0.1";    //default ip loop back
    string _port = "27183";

    // device config and server
    int _bitrate = 100000000; // default 200000000
    int _resolution = 480; // default 720
    thread _device_server_thread;
    int _socket;        //connect socket
    int _socket_fd;     //listen socket

    // method
    string run_exec(string origin_cmd);
    bool get_device_name();
    bool push_server_to_device(string server_path);
    bool reverse_config(string domain_socket_name, string local_port);
    bool start_device();
    bool stop_device();
public:
    int get_socket();
    bool start();
};