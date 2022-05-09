#include <iostream>
#include <regex>
#include <bits/stdc++.h>

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

    // method
    string exec(string origin_cmd);
    bool push_server_to_device(string server_path);

public:
    bool get_device_name();
};