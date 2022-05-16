
#include <iostream>
#include <regex>
#include <bits/stdc++.h>
#include <thread>
#include <stdio.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
class Server
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
    string _port = "20000";

    // device config and server
    int _bitrate = 100000000; // default 200000000
    int _resolution = 1280; // default 720
    thread _device_server_thread;
    int _video_socket;        //connect socket
    int _socket_fd;     //listen socket

    // method
    string run_exec(string origin_cmd);
    enum ServerStat{
        INIT_STAT,                //initial state
        PUSH_SUCCESS,            //Successfully push the server to the mobile phone
        REVERSE_SUCCESS,        //Successfully start reverse proxy
        RUN_SUCCESS            //The server program is already running on the phone
    };
    ServerStat _server_stat = INIT_STAT;
    bool get_device_name();
    bool push_server_to_device(string server_path);
    bool reverse_config(string domain_socket_name, string local_port);
    bool start_run_in_device();
    bool video_socket_init(string id,string port);
    bool aftermath();
    bool remove_server_from_device(string server_path);
    bool remove_reverse(string domain);

    // winsock
    WSADATA wsa;
	SOCKET s;
public:
    bool setDevice(int device_id, string ip_address, string port, int bitrate, int resolution);
    vector<string> getDevice_list();
    int get_socket();
    bool start_step();
    bool stop_device();
    bool adb_kill();
};