
#ifndef ESP32_SOFTWARE_GPS_H
#define ESP32_SOFTWARE_GPS_H

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

void GPS_Init();
void task_gps(void *arg);
void GPS_warmStart();
void GPS_configMessages();

#endif //ESP32_SOFTWARE_GPS_H
