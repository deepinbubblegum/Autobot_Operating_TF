#include <frame_buffer.hpp>

bool FrameBuffer::init(int height, int width)
{
    int size = height * width * 3; // rgb
    _buffer.resize(BUFFER_LEN, nullptr);
    _buffer_stat.resize(BUFFER_LEN, EMPTY);
    for (int i = 0; i < BUFFER_LEN; ++i)
    {
        _buffer[i] = new unsigned char[size];
    }
    _origin_height = height;
    _origin_width = width;
    return true;
}

int FrameBuffer::get_useful_frame_number(){
    int len = 0;
    for(int i=0;i<BUFFER_LEN;++i){
        if(_buffer_stat[i] != EMPTY)
            len++;
    }
    return len;
}

bool FrameBuffer::get_mat(cv::Mat& mat_out,ReadMode read_mode){
    unique_lock<mutex> lock(_buffer_info_lock);
    while(get_useful_frame_number() == 0){
        _not_empty.wait(lock);
    }
    
    //The next bit may be empty, at this time it can be determined that there is available data
    //Search for the number of available data frames according to the producer's fill data direction
    do{
        _read_point = (_read_point + 1) % BUFFER_LEN;
    }while(_buffer_stat[_read_point] == EMPTY);

    //data is available
    if(_buffer_stat[_read_point] == ORIGIN){
        cv::Mat t(_origin_height,_origin_width,CV_8UC3);
        t.data = _buffer[_read_point];
        mat_out = t;
    }
    else if(_buffer_stat[_read_point] == ROTATED){
        cv::Mat t(_origin_width,_origin_height,CV_8UC3);
        t.data = _buffer[_read_point];
        mat_out = t;
    }
    else{
        cout << "thread synchronization exception" << endl;
        return false;
    }
    _buffer_stat[_read_point] = EMPTY;
    return true;
}

//Producer uses
//Mark the decoded frame in the buffer, with resolution information to handle horizontal and vertical screen conversion
//Circular use of buffers, no buffer full
bool FrameBuffer::push_frame(int height,int width){
    _this_time = chrono::high_resolution_clock::now();
	chrono::duration<double, std::milli> tm = _this_time - _last_time;	// 毫秒
	_last_time = _this_time;
    //cout << "Single frame transmission plus decoding time" << tm.count() << "ms" << std::endl;
    {
        unique_lock<mutex> lock(_buffer_info_lock);
        if(height == _origin_height && width == _origin_width){
            //Not rotated and image available
            _buffer_stat[_write_point] = ORIGIN;
        }
        else{
            //Rotated image available
            _buffer_stat[_write_point] = ROTATED;
        }
        _write_point = (_write_point+1)%BUFFER_LEN;
    }
    //Unlock first and then notify to prevent the mutex from not being acquired after notification and sleeping again
    _not_empty.notify_all();
    return true;
}
