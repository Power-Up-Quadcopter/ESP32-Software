
#ifndef DRONE_SOFTWARE_CONTROL_H
#define DRONE_SOFTWARE_CONTROL_H

#include <cstdint>

namespace Control{

    /** Component initialization function. Call before using this component.
   */
    void init();

    /** Update the joystick axis values.
     *
     * @param in A pointer to a 4 byte array containing the axis values.
     */
    void updateAxes(const uint8_t* in);

    /** Update the joystick button values.
     *
     * @param in A pointer to a 2 byte array containing the button values.
     */
    void updateButtons(const uint8_t* in);

    /** Update the joystick connect disconnect status.
     *
     * @param connected False for disconnect, true for connect.
     */
    void updateStatus(bool connected);


}

#endif //DRONE_SOFTWARE_CONTROL_H
