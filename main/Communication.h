
#ifndef DRONE_SOFTWARE_COMMUNICATION_H
#define DRONE_SOFTWARE_COMMUNICATION_H

#include <string>

namespace Talk{

    //takes in char* buffer from TCP/UDP packets and does things
    //puts output bytes in the out parameter
    //returns len of output bytes,
    //if len==0 dont send anything
    //if len==-1 command unrecognized
    int parse(const uint8_t* in, uint8_t* out, int len);



}

#endif //DRONE_SOFTWARE_COMMUNICATION_H