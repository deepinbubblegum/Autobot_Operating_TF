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