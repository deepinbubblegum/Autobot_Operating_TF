#include <controller.hpp>

void Controller::exec(string input){
    system("dir");
}

// example data
// /dev/input/event[number]: 0003 x position 
// /dev/input/event4: 0003 y position

// Set coordinates

bool Controller::setCoordinates(){
    return true;
}

bool Controller::touch(){
    return true;
}