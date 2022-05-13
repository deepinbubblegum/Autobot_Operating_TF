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

bool Stream::decoder_init()
{
    _codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!_codec)
    {
        cout << "Decoder initialization failed" << endl;
        return false;
    }
    _codec_context = avcodec_alloc_context3(_codec);
    if (!_codec_context)
    {
        cout << "Decoder context initialization failed" << endl;
        return false;
    }
    if (avcodec_open2(_codec_context, _codec, NULL) < 0)
    {
        cout << "Decoder startup failed" << endl;
        return false;
    }
    _parser = av_parser_init(AV_CODEC_ID_H264);
    if (!_parser)
    {
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

void Stream::frame_creat_run()
{
    while (!_need_stop)
    {
        AVPacket packet;
        bool ok = recv_packet(&packet);
        if (!ok)
        {
            cout << "Single frame data acquisition failed" << endl;
            return;
        }
        ok = packet_manager(&packet);
        av_packet_unref(&packet);
        if (!ok)
        {
            cout << "Video frame acquisition failed" << endl;
            return;
        }
    }
}

void Stream::run_recv(Stream *video_stream)
{
    signal(SIGINT, interrupt_handle);
    video_stream->frame_creat_run();
}

bool Stream::get_img(cv::Mat &mat_out)
{
    return _frame_buffer.get_mat(mat_out);
}

bool Stream::stop_frame_creat()
{
    _need_stop = true;
    return true;
}

// Read h264 raw stream from socket
bool Stream::recv_packet(AVPacket *packet)
{
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
    int r = recv(_video_socket, (char *)&_header, HEADER_SIZE, 0);
    if (r < HEADER_SIZE)
    {
        cout << "Failed to obtain single frame header information" << endl;
        return false;
    }

    // get timestamp
    uint64_t pts = buffer_read_64byte(_header);
    // Get frame length information
    uint32_t len = buffer_read_32byte((uint8_t *)&_header[8]);
    assert(pts == NO_PTS || (pts & 0x8000000000000000) == 0);
    assert(len);

    // Allocate space for newly received packets
    if (av_new_packet(packet, static_cast<int>(len)))
    {
        cout << "Failed to allocate space for package" << endl;
        return false;
    }
    // Read data based on packet length
    int recv_now = 0;
    int len_all = static_cast<int>(len);
    while (recv_now < len_all)
    {
        int need_len = len_all - recv_now;
        r = recv(_video_socket, (char *)packet->data + recv_now, need_len, 0);
        recv_now += r;
    }
    packet->pts = pts != NO_PTS ? static_cast<int64_t>(pts) : static_cast<int64_t>(AV_NOPTS_VALUE);
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

bool Stream::packet_manager(AVPacket *packet)
{
    bool is_config = packet->pts == AV_NOPTS_VALUE;
    // config packets cannot be decoded immediately,Does not contain complete data information
    // It needs to cooperate with the frame information after that to decode
    if (_has_pending || is_config)
    {
        int offset;
        // There are delays,Continue the previously buffered frame
        if (_has_pending)
        {
            offset = _pending.size;
            // Only the expansion of the memory space is performed, no data filling is performed
            if (av_grow_packet(&_pending, packet->size))
            {
                cout << "Continuation packet failed" << endl;
                return false;
            }
        }
        // When the frame was not cached before,new package
        else
        {
            offset = 0;
            if (av_new_packet(&_pending, packet->size))
            {
                cout << "Failed to create new package" << endl;
                return false;
            }
            _has_pending = true;
        }

        // Add the newly read content to the pending
        memcpy(_pending.data + offset, packet->data, static_cast<unsigned int>(packet->size));

        // unconfigured frame,Information maintenance after the connection is completed
        if (!is_config)
        {
            _pending.pts = packet->pts;
            _pending.dts = packet->dts;
            _pending.flags = packet->flags;
            packet = &_pending;
        }
    }

    if (!is_config)
    {
        // There is data generation available for packet parsing
        bool ok = parse_packet_to_frame(packet, _yuv_frame);
        if (_has_pending)
        {
            // At this time, there is also an abnormal situation with pend,Error freeing resources
            _has_pending = false;
            av_packet_unref(&_pending);
        }
        if (!ok)
        {
            cout << "Error parsing image frame from package" << endl;
            return false;
        }
    }
    return true;
}

// Parse the video frame number from the data packet
bool Stream::parse_packet_to_frame(AVPacket *packet, AVFrame *decoding_frame)
{
    unsigned char *out_data = nullptr;
    int out_len = 0;
    // Split data into frames from the original data
    int r = av_parser_parse2(_parser, _codec_context, &out_data, &out_len, packet->data, packet->size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, -1);
    if (r != packet->size || packet->size != out_len)
    {
        cout << "Parse error" << endl;
        return false;
    }
    if (_parser->key_frame == 1)
    {
        packet->flags |= AV_PKT_FLAG_KEY;
    }

    // Process the extracted single frame data
    if (!_codec_context || !decoding_frame)
    {
        cout << "Not available for decoding" << endl;
        return false;
    }
    // Decode from h264 to yuv and yuv to rgb,The processing time is shorter than 5ms
    // External image processing time consumes more,Therefore, the decoding process is all carried out in this thread
    // h264 to yuv

    int ret = -1;
    ret = avcodec_send_packet(_codec_context, packet);
    if (ret < 0)
        return false;
    ret = avcodec_receive_frame(_codec_context, decoding_frame);
    if (ret < 0)
        return false;

    // yuv to rgb
    // TODO:runtime optimization
    AVFrame *rgb_frame = nullptr;
    rgb_frame = av_frame_alloc();
    uint8_t *output_buffer = (uint8_t *)_frame_buffer.get_buffer();
    avpicture_fill((AVPicture *)rgb_frame, output_buffer, AV_PIX_FMT_BGR24, _codec_context->width, _codec_context->height);
    SwsContext *img_convert_ctx = sws_getContext(_codec_context->width, _codec_context->height, _codec_context->pix_fmt, _codec_context->width, _codec_context->height, AV_PIX_FMT_BGR24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
    sws_scale(img_convert_ctx, decoding_frame->data, decoding_frame->linesize, 0, _codec_context->height, rgb_frame->data, rgb_frame->linesize);
    _frame_buffer.push_frame(_codec_context->height, _codec_context->width);
    av_free(rgb_frame);
    return true;
}