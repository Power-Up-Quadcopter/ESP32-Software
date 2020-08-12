
#ifndef DRONE_SOFTWARE_COMMUNICATION_H
#define DRONE_SOFTWARE_COMMUNICATION_H

#include <string>

namespace Talk{

    extern bool receive; /*!< Global to keep track of the timeout state. */

    /** Parse function for TCP/UDP packets.
     *
     *  @param in Pointer to array containing packet data
     *  @param len Length of the packet data
     *  @param out Packet to send back to sender if there is one.
     *
     *  @return
     *      <0 for error codes,
     *      0 for no output,
     *      >0 for output length
    */
    int parse(const uint8_t* in, uint8_t* out, int len);


    /** Send an error msg over TCP. High priority.
     *
     * @param msg Message to be sent
     */
    void sendError(std::string msg);

    /** Send a debug message over UDP.
     *
     * @param msg Message to be sent
     */
    void sendNorm(std::string msg);
}

#endif //DRONE_SOFTWARE_COMMUNICATION_H
