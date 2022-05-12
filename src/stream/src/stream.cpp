#include <stream.hpp>

int socket_fd_g = -1;
void Stream::interrupt_handle(int sig)
{
    if (socket_fd_g != -1)
        shutdown(socket_fd_g, SD_BOTH);
    exit(0);
}

bool Stream::stream_init(int video_socket)
{
    if (_video_socket < 0)
    {
        cout << "invalid socket" << endl;
        return false;
    }
    _video_socket = video_socket;
    if (!get_device_info())
    {
        cout << "Failed to get device info" << endl;
        return false;
    }
    if (!ffmpeg_init())
    {
        return false;
    }
    if (!decoder_init())
    {
        cout << "Decoder initialization failed" << endl;
        return false;
    }
    _frame_buffer.init(_resolution_height, _resolution_width);
    _need_stop = false;
    _yuv_frame = av_frame_alloc();
    _recv_thread = thread(run_recv, this);
    return true;
}

// The socket establishment only obtains the device information at the beginning
bool Stream::get_device_info()
{
    unsigned char buf[DEVICE_NAME_FIELD_LENGTH + 4];
    int n = recv(_video_socket, (char *)&buf, sizeof(buf), 0);
    if (n != DEVICE_NAME_FIELD_LENGTH + 4)
    {
        cout << "Failed to get device info" << endl;
        return false;
    }
    buf[DEVICE_NAME_FIELD_LENGTH - 1] = '\0';
    _device_name = (char *)buf;
    _resolution_width = (buf[DEVICE_NAME_FIELD_LENGTH] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 1];
    _resolution_height = (buf[DEVICE_NAME_FIELD_LENGTH + 2] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 3];
    cout << "Device name:" << _device_name << endl;
    cout << "Resolution    :" << _resolution_width << " * " << _resolution_height << endl;
    return true;
}

bool Stream::decoder_init(){
    _codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if(!_codec){
        cout <<  "Decoder initialization failed" <<endl;
       return false;
    }
    _codec_context = avcodec_alloc_context3(_codec);
    if (!_codec_context) {
        cout << "Decoder context initialization failed" << endl;
        return false;
    }
    if (avcodec_open2(_codec_context, _codec, NULL) < 0) {
        cout << "Decoder startup failed" << endl;
        return false;
    }
    _parser = av_parser_init(AV_CODEC_ID_H264);
    if (!_parser) {
        cout << "Interpreter initialization failed" << endl;
        return false;
    }
    _parser->flags |= PARSER_FLAG_COMPLETE_FRAMES;
    return true;
}

bool Stream::ffmpeg_init()
{
    // av_register_all(); // deprecated in ffmpeg  >= 4.0
    if (avformat_network_init())
    {
        cout << "ffmpeg initialization failed" << endl;
        return false;
    }
    return true;
}

void Stream::frame_creat_run(){
    while(!_need_stop){
        AVPacket packet;
        bool ok = recv_packet(&packet);
        if (!ok) {
            cout << "Single frame data acquisition failed" << endl;
            return;
        }
        ok = packet_manager(&packet);
        av_packet_unref(&packet);
        if(!ok){
            cout << "Video frame acquisition failed" << endl;
            return;
        }
    }
}

void Stream::run_recv(Stream* video_stream){
    signal(SIGINT, interrupt_handle);
    video_stream->frame_creat_run();
}

bool Stream::get_img(cv::Mat& mat_out){
    return _frame_buffer.get_mat(mat_out);
}

bool Stream::stop_frame_creat(){
    _need_stop = true;
    return true;
}

//Read h264 raw stream from socket
bool Stream::recv_packet(AVPacket* packet){
    // The video stream contains raw packets, without time information. When we
    // record, we retrieve the timestamps separately, from a "meta" header
    // added by the server before each raw packet.
    //
    // The "meta" header length is 12 bytes:
    // [. . . . . . . .|. . . .]. . . . . . . . . . . . . . . ...
    //  <-------------> <-----> <-----------------------------...
    //        PTS        packet        raw packet
    //                    size
    //
    // It is followed by <packet_size> bytes containing the packet/frame.
    int r =  recv(_video_socket, (char *)&_header, HEADER_SIZE,0);
    if(r < HEADER_SIZE){
        cout << "Failed to obtain single frame header information" << endl;
       return false;
    }

    // // // ยังไม่เสร็จ
    // //get timestamp
    // uint64_t pts = buffer_read_64byte(_header);
    // //Get frame length information
    // uint32_t len = buffer_read_32byte((uint8_t*)&_header[8]);
    // assert(pts == NO_PTS || (pts & 0x8000000000000000) == 0);
    // assert(len);
    
    // //Allocate space for newly received packets
    // if (av_new_packet(packet, static_cast<int>(len))) {
    //     cout << "Failed to allocate space for package" << endl;
    //     return false;
    // }
    // //Read data based on packet length
    // int recv_now = 0;
    // int len_all = static_cast<int>(len);
    // while(recv_now < len_all){
    //     int need_len = len_all - recv_now; 
    //     r =  recv(_video_socket, packet->data + recv_now, need_len, 0);
    //     recv_now += r;
    // }
    // packet->pts = pts != NO_PTS ? static_cast<int64_t>(pts) : static_cast<int64_t>(AV_NOPTS_VALUE);
    return true;
}

// Utility function for header decoding
uint32_t Stream::buffer_read_32byte(uint8_t *buf)
{
    return static_cast<uint32_t>((buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]);
}

uint64_t Stream::buffer_read_64byte(uint8_t *buf)
{
    uint32_t msb = buffer_read_32byte(buf);
    uint32_t lsb = buffer_read_32byte(&buf[4]);
    return (static_cast<uint64_t>(msb) << 32) | lsb;
}

bool Stream::packet_manager(AVPacket* packet){
    return true;
}