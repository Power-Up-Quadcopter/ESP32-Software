
#ifndef DRONE_SOFTWARE_SD_H
#define DRONE_SOFTWARE_SD_H

#define SD_DAT0 2
#define SD_DAT1 4
#define SD_DAT2 12
#define SD_DAT3 13
#define SD_CLK 14
#define SD_CMD 15

#define MOUNT_POINT "/sdcard"

namespace SD{

    /** Component initialization function. Call before using this component.
     *  @note Calling this function may break debug mode.
    */
    void init();

}

#endif //DRONE_SOFTWARE_SD_H
