#include <iostream>

// Constant
#define BUFSIZE 4096
using namespace std;
class Controller
{
private:
    void exec(string input);

public:
    bool setCoordinates();
    bool touch();
};