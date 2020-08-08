
#ifndef DRONE_SOFTWARE_CONTROL_H
#define DRONE_SOFTWARE_CONTROL_H

#include <cstdint>

namespace Control{


    void updateAxes(const uint8_t* in);
    void updateButtons(const uint8_t* in);
    void updateStatus(bool connected);

    void init();
}

#endif //DRONE_SOFTWARE_CONTROL_H
