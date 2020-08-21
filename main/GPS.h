
#ifndef ESP32_SOFTWARE_GPS_H
#define ESP32_SOFTWARE_GPS_H

#include <string>

//true if you dont have warm start values saved in your esp32
#define DEFAULT_DATA true

#define GPSTASK &task_Alec
//#define GPSTASK &task_Allen

typedef enum {
    ALL = 0,
    TIME_DATE = 1,
    LOCATION = 2,
    SPEED_DIR = 3,
} GPS_send_type_t;

typedef enum {
    TIMESTAMP_ = 0,
    STATUS_ = 1,
    NS_ = 2,
    LATDEGREES_ = 3,
    LATMINUTES_ = 4,
    LATPARTMINS_ = 5,
    EW_ = 6,
    LONGDEGREES_ = 7,
    LONGMINUTES_ = 8,
    LONGPARTMINS_ = 9,
    SPEED_ = 10,
    COURSEMADEGOOD_ = 11,
    DAY_ = 12,
    MONTH_ = 13,
    YEAR_ = 14,
    MAGVAR_ = 15,
    MAGVARDIR_ = 16,
} GPS_globals_ind;

namespace GPS {

    //Lots of globals
    extern int timeStamp;      /*!< < UTC timestamp in seconds. */
    extern uint8_t status;     /*!< 1 for valid read, 0 for warning */
    extern uint8_t NS;         /*!< 0 North, 1 South */
    extern int latDegrees;      /*!< Degrees latitude */
    extern int latMinutes;      /*!< Minutes latitude */
    extern int latPartMins;    /*!< Decimal minutes latitude, fixed point resolution of .00001 */
    extern uint8_t EW;         /*!< 0 East, 1 West */
    extern int longDegrees;     /*!< Degrees longitude */
    extern int longMinutes;     /*!< Minutes longitude */
    extern int longPartMins;   /*!< Decimal minutes longitude, fixed point resolution of .00001 */
    extern int speed;          /*!< Speed in knots, fixed point resolution of 0.1 */
    extern int courseMadeGood; /*!< Direction of actual travel, NOT heading, fixed point resolution of 0.1 */
    extern uint8_t day;     /*!< Current day */
    extern uint8_t month;  /*!< Current month */
    extern int year;       /*!< Current year */
    extern int magVar;         /*!< Magnetic variation, fixed point resolution of 0.1 */
    extern uint8_t magVarDir;  /*!< Magnetic variation direction, 0 East, 1 West */

    /** Component initialization function. Call before using this component.
     *
    *  @param isWarmStart True if the GPS should use a warm start, false for a cold start.
    */
    extern void init(bool isWarmStart);

    /** Send a command over UART to the GPS.
     *
     * @param msg GPS command without the $, *, or checksum
     */
    void sendGPS(std::string msg);

    /** Get a string representing the various debug globals
     *
    *  @param What category of data the function should return
     *
     *
     *  @return A string containing various GPS globals
    */
    extern std::string send(GPS_send_type_t type);


    /** Exports GPS position telemetry for use in UDP packets.
     *
     * @Note See the communication protocol document in drive for info on the packet format.
     *
     * @return String containing packet data to send over UDP.
     */
    std::string posTelemety();

    /** Exports GPS misc telemetry for use in UDP packets.
     *
     * @Note See the communication protocol document in drive for info on the packet format.
     *
     * @return String containing packet data to send over UDP.
     */
    std::string miscTelemety();

}

#endif //ESP32_SOFTWARE_GPS_H
