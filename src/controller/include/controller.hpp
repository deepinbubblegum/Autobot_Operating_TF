#include <iostream>

using namespace std;
class Controller
{
private:
    string exec_result;
    string run_exec(string origin_cmd);
    string findDeviceInput(string device);

public:
    void test(string input);
    bool setCoordinates();
    bool touch();
};