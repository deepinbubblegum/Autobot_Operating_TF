#include <iostream>
#include <chrono>
#include <thread>
#include <bitset>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include <ws2tcpip.h>

#include <frame_buffer.hpp>

extern "C"{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>   
}
#define NO_PTS UINT64_MAX
#define HEADER_SIZE 12
#define DEVICE_NAME_FIELD_LENGTH 64
using namespace std;

class Stream
{
public:
    //Initialize socket and related decoders
    bool stream_init(int video_socket);
    //Run the yuv frame generation thread
    void frame_creat_run();
    static void run_recv(Stream* video_stream);
    //Stop the original frame acquisition thread and release related resources
    bool stop_frame_creat();
    //For external image processing thread to get image
    bool get_img(cv::Mat& mat_out);
private:
    bool ffmpeg_init();
    bool decoder_init();
    bool get_device_info();
    bool recv_packet(AVPacket* packet);
    // get video frame from packet
    bool packet_manager(AVPacket* packet);
    // Parse the image frame from the data packet
    bool parse_packet_to_frame(AVPacket* packet,AVFrame* decoding_frame);
    
    //Single packet data header parsing tool function
    uint64_t buffer_read_64byte(uint8_t* buf);
    uint32_t buffer_read_32byte(uint8_t* buf);

    //In response to ctrl c exit, release resources
    static void interrupt_handle(int sig);

    int _video_socket;
    string _device_name;
    int _resolution_width, _resolution_height;

    AVCodec* _codec = nullptr;                  //decoder
    AVCodecContext* _codec_context= nullptr;   //Decoder context
    AVCodecParserContext* _parser = nullptr;  //for data stitching, Get a single frame of data
    bool _need_stop;
    //The header information of a single packet of data will be called frequently, in order to avoid wasting memory allocation time and allocate it in advance
    unsigned char _header[HEADER_SIZE];
    
    //h264 data packets need to be spliced, use this information to record whether splicing is required
    bool _has_pending = false;
    //Buffer to use when stitching is required
    AVPacket _pending;
    //yuv whole frame information
    AVFrame* _yuv_frame = nullptr;
    
    FrameBuffer  _frame_buffer;

    //data receiving thread
    thread _recv_thread;
};
