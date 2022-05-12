#include <stream.hpp>

int socket_fd_g = -1;
void Stream::interrupt_handle(int sig){
    if(socket_fd_g != -1)
        shutdown(socket_fd_g, SD_BOTH);
    exit(0);
}

bool Stream::stream_init(int video_socket){
    if(_video_socket < 0){
        cout << "invalid socket" << endl;
        return false;
    }
    _video_socket = video_socket;
    if(!get_device_info()){
        cout << "Failed to get device info" << endl;
        return false;
    }
    return true;
}

//The socket establishment only obtains the device information at the beginning
bool Stream::get_device_info(){
    unsigned char buf[DEVICE_NAME_FIELD_LENGTH + 4];
    // int buflen;
    // buflen = strlen(buf);
    // int size_buff = DEVICE_NAME_FIELD_LENGTH + 4;
    // vector<char> buf(size_buff);
    int n = recv(_video_socket, (char *)&buf, sizeof(buf), 0);
    if(n != DEVICE_NAME_FIELD_LENGTH + 4){
        cout << "Failed to get device info" << endl;
        return false;
    }
    buf[DEVICE_NAME_FIELD_LENGTH - 1] = '\0'; // in case the client sends garbage
    // strcpy is safe here, since name contains at least DEVICE_NAME_FIELD_LENGTH bytes
    // and strlen(buf) < DEVICE_NAME_FIELD_LENGTH
    _device_name = (char *)buf;
    _resolution_width = (buf[DEVICE_NAME_FIELD_LENGTH] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 1];
    _resolution_height = (buf[DEVICE_NAME_FIELD_LENGTH + 2] << 8) | buf[DEVICE_NAME_FIELD_LENGTH + 3];
    cout << "Device name:"<< _device_name << endl;
    cout << "Resolution    :" << _resolution_width << " * "  << _resolution_height << endl;
    return true;
}