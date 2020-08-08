
#ifndef ESP32_SOFTWARE_GPS_H
#define ESP32_SOFTWARE_GPS_H

#include <string>

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
    extern int timeStamp;     //UTC timestamp in seconds
    extern uint8_t status;     //1 valid, 0 warning
    extern uint8_t NS;         //0 North, 1 South
    extern int latDegrees;
    extern int latMinutes;
    extern int latPartMins;    //fixed point resolution of .00001
    extern uint8_t EW;         //0 East, 1 West
    extern int longDegrees;
    extern int longMinutes;
    extern int longPartMins;   //fixed point resolution of .00001
    extern int speed;          //speed in knots, fixed point resolution of 0.1
    extern int courseMadeGood; //direction of actual travel, NOT heading, fixed point resolution of 0.1
    extern uint8_t day;
    extern uint8_t month;
    extern int year;
    extern int magVar;         //magnetic variation, fixed point resolution of 0.1
    extern uint8_t magVarDir;  //magnetic variation direction, 0 East, 1 West

    void sendGPS(std::string msg);

    extern void init(bool isWarmStart);

    extern std::string send(GPS_send_type_t type);

    //exports GPS position telemetry for use in UDP packets
    //see Communication protocol document in drive
    std::string posTelemety();

    //exports GPS misc telemetry for use in UDP packets
    //see Communication protocol document in drive
    std::string miscTelemety();

    extern void task_Allen(void *arg);
}

#endif //ESP32_SOFTWARE_GPS_H
