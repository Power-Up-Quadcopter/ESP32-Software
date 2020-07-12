
#include <driver/uart.h>
#include <esp_log.h>
#include "GPS.h"
#include <sstream>
#include <string.h>
#include "Wifi.h"

#define GPS_LOOP_STACK_SIZE 2048
#define GPSRX 33
#define GPSTX 32

using namespace std;

QueueHandle_t uart_queue;
const uart_port_t uart_num = UART_NUM_2;

//see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html

const string warmStart_lat = "3017.057,N";
const string warmStart_lon = "09744.648,W";
const string warmStart_alt = "0230";
const string warmStart_date = "11,07,2020";
const string warmStart_time = "18,00,00";


bool GPSready = false;

int timeStamp = 0;      //UTC timestamp in seconds
uint8_t status = 1;     //1 valid, 0 warning
uint8_t NS = 0;         //0 North, 1 South
int latDegrees = 0;
int latMinutes = 0;
int latPartMins = 0;    //fixed point resolution of .00001
uint8_t EW = 0;         //0 East, 1 West
int longDegrees = 0;
int longMinutes = 0;
int longPartMins = 0;   //fixed point resolution of .00001
int speed = 0;          //speed in knots, fixed point resolution of 0.1
int courseMadeGood = 0; //direction of actual travel, NOT heading, fixed point resolution of 0.1
uint8_t day = 1;
uint8_t month = 1;
int year = 2020;
int magVar = 0;         //magnetic variation, fixed point resolution of 0.1
uint8_t magVarDir = 0;  //magnetic variation direction, 0 East, 1 West
string infoStr[17]; //array of string ptrs corresponding with info globals above, in the same order as the globals

void GPS_Loop(void* ptr);
void GPS_Task_Alec(void* ptr);
void GPS_Task_Allen(void* arg);

void sendGPS(string msg) {
    uint8_t checksum = msg.data()[0];
    for(int i = 1; i < msg.length(); i++) {
        checksum ^= msg.data()[i];
    }
    stringstream sstream;
    sstream << hex << checksum;
    string toSend = "$";
    toSend.append(msg);
    toSend.append("*");
    toSend.append(sstream.str());
    printf("GPS SEND: %s\n", toSend.data());
    toSend.append("\r\n");

    uart_write_bytes(UART_NUM_2, toSend.data(), toSend.length());
}


void GPS_warmStart() {
    sendGPS("PSTMGPSRESTART");

    string initStr = "PSTMINITGPS,";
    initStr.append(warmStart_lat);
    initStr.append(",");
    initStr.append(warmStart_lon);
    initStr.append(",");
    initStr.append(warmStart_alt);
    initStr.append(",");
    initStr.append(warmStart_date);
    initStr.append(",");
    initStr.append(warmStart_time);
    sendGPS(initStr);

    sendGPS("PSTMWARM");
}

void GPS_configMessages() {
    sendGPS("PSTMSETPAR,1201,10,2");   // GPVTG off
    sendGPS("PSTMSETPAR,1201,800000,2");   // PSTMCPU off
    sendGPS("PSTMSETPAR,1201,40,2");   // GPRMC off
    sendGPS("PSTMSETPAR,1201,40,2");   // GPRMC off
    sendGPS("PSTMSETPAR,1201,20,2");   // PSTMNOISE off
    sendGPS("PSTMSETPAR,1201,4,1");   // GNGSA on
    sendGPS("PSTMSETPAR,1201,80000,1");   // GPGSV on
    sendGPS("PSTMSETPAR,1201,100000,1");   // GPGLL on
    sendGPS("PSTMSETPAR,1201,1000000,1");   // GPZDA on
}

void GPS_Init(bool warmStart){
    uart_config_t uart_config = {
            .baud_rate = 9600,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .rx_flow_ctrl_thresh = 122,
            .use_ref_tick = 0
    };


    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    // Set rx and tx pins
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_2, GPSTX, GPSRX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Setup UART buffered IO with event queue
    const int uart_buffer_size = (1024 * 2);

    // Install UART driver using an event queue here
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_2, uart_buffer_size, \
                                        uart_buffer_size, 10, &uart_queue, 0));

    GPS_configMessages();
    if(warmStart) {GPS_warmStart();}


    xTaskCreate(GPSTASK, "GPSLoop", GPS_LOOP_STACK_SIZE, NULL, 5, NULL);
}


void GPS_sendTCP(GPS_send_type_t type){
    string tosend = "GPS_INFO\n";
    tosend.append(infoStr[STATUS_] + "\n");
    if(type==TIME_DATE || type==ALL){
        tosend.append("TIME_DATE\n");
        tosend.append(infoStr[TIMESTAMP_] + "\n");
        tosend.append(infoStr[DAY_] + "\n");
        tosend.append(infoStr[MONTH_] + "\n");
        tosend.append(infoStr[YEAR_] + "\n");
    }
    if(type==LOCATION || type==ALL){
        tosend.append("LOCATION\n");
        tosend.append(infoStr[NS_] + "\n");
        tosend.append(infoStr[LATDEGREES_] + "\n");
        tosend.append(infoStr[LATMINUTES_] + "\n");
        tosend.append(infoStr[LATPARTMINS_] + "\n");
        tosend.append(infoStr[EW_] + "\n");
        tosend.append(infoStr[LONGDEGREES_] + "\n");
        tosend.append(infoStr[LONGMINUTES_] + "\n");
        tosend.append(infoStr[LONGPARTMINS_] + "\n");

    }
    if(type==SPEED_DIR || type==ALL){
        tosend.append("SPEED_DIR\n");
        tosend.append(infoStr[SPEED_] + "\n");
        tosend.append(infoStr[COURSEMADEGOOD_] + "\n");
        tosend.append(infoStr[MAGVAR_] + "\n");
        tosend.append(infoStr[MAGVARDIR_] + "\n");
    }

    Wifi_sendTCP((char*) tosend.c_str(), tosend.size());
}

//splits given string into lines
//ignores lines that reach eof before newline
//ignores lines that dont start with $
//returns number of generated lines
int getLines(const uint8_t* in, char** out){
    int pos = 0;
    for(int i = 0; i<7; i++){

        int len = 0;
        if(in[pos]!='$'){return i;}
        for(; in[len+pos]!='\r'; len++){
            if(in[len+pos] == 0){return i;}
        }
        out[i] = (char*) malloc(len+1);

        for(int j=0; j<len; j++, pos++){
            out[i][j] = in[pos];
        }
        out[i][len] = 0;
        pos +=2;
    }
    return 7;
}

//splits cmd into separate strings using commas
//ends when an asterisk is encountered
int cmdHelper(char* in, char** out){
    int outPos = 0;
    int outPos2 = 0;
    out[0] = (char*) malloc(20);
    for(int i=0; in[i]!=0; i++){
        while(in[i] == ','){
            out[outPos][outPos2] = 0;
            out[++outPos] = (char*) malloc(20);
            outPos2 = 0;
            i++;
        }
        if(in[i] == '*'){
            out[outPos][outPos2] = 0;
            return outPos;
        }
        out[outPos][outPos2++] = in[i];
    }
    return outPos;
}


//interprets a cmd and sorts its info into relevant variables
void cmdParse(char* cmd){
    char** args = (char**) malloc(sizeof(char*) * 20);
    int numArgs = cmdHelper(cmd, args);

    if(strcmp(args[0],"$GPRMC")==0){ //time, date, position, and speed, works well enough for our application
        if(args[1][0]!=0){
            timeStamp = atoi(args[1]+4);
            args[1][4] = 0; args[1][5] = 0;
            timeStamp += 60 * atoi( args[1]+2);
            args[1][2] = 0; args[1][3] = 0;
            timeStamp += 3600 * atoi(args[1]);
        }
        if(args[2][0]!=0){
            status = args[2][0]=='A';
        }
        if(args[3][0]!=0){
            latPartMins = atoi(args[3]+5);
            latMinutes  = atoi( args[3]+2);
            args[3][2] = 0; args[3][3] = 0;
            latDegrees   =  atoi(args[3]);
        }
        if(args[4][0]!=0){
            NS = args[4][0]=='S';
        }
        if(args[5][0]!=0){
            longPartMins = atoi(args[5]+5);
            longMinutes  = atoi( args[5]+2);
            args[5][2] = 0; args[5][3] = 0;
            longPartMins   =  atoi(args[5]);
        }
        if(args[6][0]!=0){
            NS = args[6][0]=='W';
        }
        if(args[7][0]!=0){
            speed = 10 * atoi(args[7]) + atoi(args[7]+4);
        }
        if(args[8][0]!=0){
            courseMadeGood = 10 * atoi(args[8]) + atoi(args[8]+4);
        }
        if(args[9][0]!=0){
            year = atoi(args[9]+4);
            args[9][4] = 0;
            month = atoi( args[9]+2);
            args[9][2] = 0;
            day = atoi(args[9]);
        }
        if(args[10][0]!=0){
            magVar = 10 * atoi(args[10]) + atoi(args[10]+4);
        }
        if(args[11][0]!=0){
            magVarDir = args[11][0]=='W';
        }
    }
    else if(strcmp(args[0],"$GPGGA")==0){ //long, lat, satellite stats
        ;
    }
    else if(strcmp(args[0],"$GPVTG")==0){ //course relative to ground, and ground speed
        ;
    }
    else if(strcmp(args[0],"$GNGSA")==0){ //no clue, doesnt appear in the manual
        ;
    }
    else if(strcmp(args[0],"$GPGLL")==0){ //GPS long, lat
        ;
    }
    else if(strcmp(args[0],"$PSTMCPU")==0){ //pulse per second data
        ;
    }
    else{
        printf("Invalid command: %s\n", args[0]);
    }

    for(int i=0; i<numArgs; i++){free(args[i]);}
    free(args);
}

void GPS_Task_Alec(void* ptr){
    uart_event_t event;
    uint8_t* input = (uint8_t*) malloc(700);
    char* lines[7];
    int inputLen = 0;
    while(1){
        ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t *) &inputLen));
        inputLen = uart_read_bytes(UART_NUM_2, input, 699, 5/portTICK_RATE_MS);
        input[inputLen] = 0;
        int numLines = getLines(input, lines);
        for(int i=0; i<numLines; i++){
            cmdParse(lines[i]);
            free(lines[i]);
        }
        input[0] = 0;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


uint8_t data[512];
char strbuffer[512];
void GPS_Task_Allen(void *arg) {

    int length = 0;
    GPSready = true;
    while(1) {
        ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t *) &length));
        length = uart_read_bytes(uart_num, data, length, 5/portTICK_RATE_MS);
        printf("%d\n", length);
        int strIndex = 0;
        for(int i = 0; i < length; i++) {
            if (data[i] == '\r') {
                strbuffer[strIndex++] = data[i];
                printf("%c", (int8_t) data[i]);
            }
        }
        strbuffer[strIndex++] = '\n';
        strbuffer[strIndex++] = '\n';
        Wifi_sendTCP(strbuffer, strIndex-1);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}


