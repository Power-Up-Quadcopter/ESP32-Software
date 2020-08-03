
#ifndef DRONE_SOFTWARE_CONTROL_H
#define DRONE_SOFTWARE_CONTROL_H

namespace Control{


    void updateAxes(const uint8_t* in);
    void updateButtons(const uint8_t* in);
    void updateStatus(const uint8_t* in);

}

#endif //DRONE_SOFTWARE_CONTROL_H
