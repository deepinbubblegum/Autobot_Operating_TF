#include <iostream>
#include <regex>
#include <bits/stdc++.h>

using namespace std;
class adb
{
private:
    string s_adbPath;
    string exec_result;
    vector<string> device_name_list;
    string exec(const char* origin_cmd);
public:
    bool get_device_name();
};