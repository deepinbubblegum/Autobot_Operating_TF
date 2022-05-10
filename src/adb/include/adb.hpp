
#include <iostream>
#include <regex>
#include <bits/stdc++.h>
#include <thread>
#include <stdio.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define _WIN32_WINNT 0x601
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
class adb
{
private:
    // variable
    string s_adbPath;
    string exec_result;
    vector<string> device_name_list;
    int index_device = 0;

    // path scrcpy
    string server_path_file = "scrcpy-server/scrcpy-server";
    string server_device_path = "/data/local/tmp/scrcpy-server";

    string _ip ="127.0.0.1";    //default ip loop back
    string _port = "27183";

    // device config and server
    int _bitrate = 100000000; // default 200000000
    int _resolution = 480; // default 720
    thread _device_server_thread;
    int _video_socket;        //connect socket
    int _socket_fd;     //listen socket

    // method
    string run_exec(string origin_cmd);
    enum ServerStat{
        INIT_STAT,                        //初始状态
        PUSH_SUCCESS,                  //成功推送服务端到手机
        REVERSE_SUCCESS,        //成功 启动反向代理
        RUN_SUCCESS                      //服务端程序已经在手机上运行
    };
    ServerStat _server_stat = INIT_STAT;
    bool get_device_name();
    bool push_server_to_device(string server_path);
    bool reverse_config(string domain_socket_name, string local_port);
    bool start_device();
    bool stop_device();
    bool video_socket_init(string id,string port);

    // winsock
    WSADATA wsa;
	SOCKET s;
public:
    int get_socket();
    bool start();
};