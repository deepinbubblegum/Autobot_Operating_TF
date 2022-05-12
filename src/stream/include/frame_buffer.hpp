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
    enum ReadMode{
        IN_ORDER, //sequential read
        REAL_TIME //real-time priority, read the latest, and move the pointer to ignore what was produced earlier
    };
    bool get_mat(cv::Mat& mat_out,ReadMode read_mode = IN_ORDER);
    bool push_frame(int height,int width);
private:
    // Get the number of currently available frames
    int get_useful_frame_number();
    // for storing buffer information
    vector<unsigned char*>  _buffer;
    //used to record the status of each buffer
    //0 represents invalid data, 1 represents the original dividing frequency data, and 2 represents the data after the original resolution is rotated by 90
    enum BufferStat{
        EMPTY,          //unavailable
        ORIGIN,        //native resolution
        ROTATED       //Image is rotated
    };
    vector<int> _buffer_stat;
    int _origin_height,_origin_width;
    //Used to control when the buffer is empty, waiting for the producer to generate data
    condition_variable _not_empty;
    //Used to prevent buffer information from being changed at the same time
    mutex _buffer_info_lock;
    // Consumer read location and producer write location
    int _read_point = -1,_write_point=0;
    chrono::high_resolution_clock::time_point _this_time,_last_time;
};