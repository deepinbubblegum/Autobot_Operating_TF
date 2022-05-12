#include <iostream>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <vector>
#include <mutex>
#include <condition_variable>
#define BUFFER_LEN 10

using namespace std;
class FrameBuffer
{
public:
    bool init(int height,int width);

private:
    vector<unsigned char*>  _buffer;
    enum BufferStat{
        EMPTY,          //unavailable
        ORIGIN,        //native resolution
        ROTATED       //Image is rotated
    };
    vector<int> _buffer_stat;
    int _origin_height,_origin_width;
};