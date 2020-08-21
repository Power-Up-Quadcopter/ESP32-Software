
#ifndef DRONE_SOFTWARE_CONTROL_H
#define DRONE_SOFTWARE_CONTROL_H

#include <cstdint>



namespace Control{

    /** Component initialization function. Call before using this component.
   */
    void init();


    /** Thread safe way to update the PID values.
     *
     * @param type which PID controller to update
     * @param args pointer to array of 3 floats, containing to P,I,D gains
     */
    void updatePID(uint8_t type, const float* args);

    /** Update the joystick axis values.
     *
     * @param in A pointer to a 4 byte array containing the axis values.
     */
    void updateAxes(const int8_t* in);

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
