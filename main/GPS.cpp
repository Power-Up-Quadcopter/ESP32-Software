
#include <sstream>
#include <string>
#include <cstring>
#include <driver/uart.h>
#include <esp_log.h>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_system.h"
#include "Wifi.h"
#include "GPS.h"

#define GPS_LOOP_STACK_SIZE 4096
#define GPSRX 33
#define GPSTX 32




//lengths in bytes of the warm start variables
//including null terminator
#define WARM_LAT 11
#define WARM_LONG 12
#define WARM_ALT 5
#define WARM_DATE 10
#define WARM_TIME 9

using namespace std;

namespace GPS {
    QueueHandle_t uart_queue;
    const uart_port_t uart_num = UART_NUM_2;
    SemaphoreHandle_t xMutex;
//see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html

    string warmStart_lat = "2964.010,N";
    string warmStart_lon = "09867.648,W";
    string warmStart_alt = "0375";
    string warmStart_date = "7,08,2020";
    string warmStart_time = "19,08,00";

#if DEFAULT_DATA
    bool prevSave = false;
#else
    bool prevSave = true;
#endif

    bool GPSready = false; //true if we have sight of at least 1 satellite

    int times = 0;
    int timeStamp = 0;
    uint8_t status = 1;
    uint8_t NS = 0;
    int latDegrees = 0;
    int latMinutes = 0;
    int latPartMins = 0;
    uint8_t EW = 0;
    int longDegrees = 0;
    int longMinutes = 0;
    int longPartMins = 0;
    int speed = 0;
    int courseMadeGood = 0;
    uint8_t day = 1;
    uint8_t month = 1;
    int year = 2020;
    int magVar = 0;
    uint8_t magVarDir = 0;
    uint8_t totSats;
    string infoStr[17]; //array of string ptrs corresponding with info globals above, in the same order as the globals

    nvs_handle_t gps_handle;

    void GPS_Loop(void *ptr);

    void task_Alec(void *ptr);


    /**Save GPS fix data to non-volatile storage for use later.
     *
     * @return Esp Idf error code
     */
    esp_err_t saveData(){
        printf("Saving to Non-Volatile Storage (NVS) handle... ");

        if(!prevSave){
            string NS_ = NS ? "N" : "S";
            string EW_ = EW ? "E" : "W";
            string latPartStr = (latPartMins>99) ? to_string(latPartMins) : "0" + to_string(latPartMins);
            string longPartStr = (longPartMins>99) ? to_string(longPartMins) : "0" + to_string(longPartMins);
            string longDegStr = (longDegrees>99) ? to_string(longDegrees) : "0" + to_string(longDegrees);
            string monthStr = (month>9) ? to_string(month) : "0" + to_string(month);


            warmStart_lat = string(to_string(latDegrees) + to_string(latMinutes) + "." + latPartStr);
            warmStart_lon = string(longDegStr + to_string(longMinutes) + "." + longPartStr);
            warmStart_date = string(to_string(day) + "," + monthStr + ","  + to_string(year));
            warmStart_time = string(to_string(timeStamp));

            prevSave = true;
        }

        esp_err_t err = nvs_open("GPS", NVS_READWRITE, &gps_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        }

        int current = 0;
        int size = WARM_LAT + WARM_LONG + WARM_ALT + WARM_DATE + WARM_TIME;
        char* out = new char[size];
        for(char const &a : warmStart_lat){
            out[current++] = a;
        }
        out[current++] = 0;
        for(char const &a : warmStart_lon){
            out[current++] = a;
        }
        out[current++] = 0;
        for(char const &a : warmStart_alt){
            out[current++] = a;
        }
        out[current++] = 0;
        for(char const &a : warmStart_date){
            out[current++] = a;
        }
        out[current++] = 0;
        for(char const &a : warmStart_time){
            out[current++] = a;
        }
        out[current++] = 0;

        err = nvs_set_blob(gps_handle, "GPS_INFO", out, size);
        if (err != ESP_OK) return err;

        err = nvs_commit(gps_handle);
        if (err != ESP_OK) return err;

        nvs_close(gps_handle);
        return ESP_OK;

    }

    /**Get GPS fix data from last time and update the globals.
     *
     * @return Esp Idf error code
     */
    esp_err_t  getSaveData(){
        printf("Reading from Non-Volatile Storage (NVS) handle... ");

        esp_err_t err = nvs_open("GPS", NVS_READWRITE, &gps_handle);
        if (err != ESP_OK) {
            printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
        }

        int out_size = 0;
        char* GPS_str = nullptr;
        err = nvs_get_blob(gps_handle, "GPS_INFO", GPS_str, (size_t*) &out_size);
        if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) return err;
        else if(err == ESP_ERR_NVS_NOT_FOUND){return ESP_OK;}

        int inc = 0;
        warmStart_lat = string(GPS_str);
        inc += WARM_LAT;
        warmStart_lon = string(GPS_str + inc);
        inc += WARM_LONG;
        warmStart_alt = string(GPS_str + inc);
        inc += WARM_ALT;
        warmStart_date = string(GPS_str + inc);
        inc += WARM_DATE;
        warmStart_time = string(GPS_str + inc);

        nvs_close(gps_handle);
        return ESP_OK;
    }

    void sendGPS(string msg) {
        uint8_t checksum = msg.c_str()[0];
        for (int i = 1; i < msg.length(); i++) {
            checksum ^= msg.c_str()[i];
        }
        stringstream sstream;
        sstream << hex << checksum;
        string toSend = "$";
        toSend.append(msg);
        toSend.append("*");
        toSend.append(sstream.str());
        printf("GPS SEND: %s\n", toSend.c_str());
        toSend.append("\r\n");
        xSemaphoreTake( xMutex, portMAX_DELAY );
        uart_write_bytes(UART_NUM_2, toSend.c_str(), toSend.length());
        uart_wait_tx_done(UART_NUM_2, 100 / portTICK_RATE_MS);
        xSemaphoreGive( xMutex );
    }

    void coldStart(){
        sendGPS("PSTMCOLD");
    }

    void warmStart() {
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
        sendGPS("PSTMSETPAR,1200,220000,1"); //enable GLONAS tracking
        sendGPS("PSTMSETPAR,1201,80040");
        sendGPS("PSTMSAVEPAR");
//        sendGPS("PSTMSETPAR,1201,80040");
//
//       sendGPS("PSTMSETPAR,1228,10,1");
//        uart_wait_tx_done(UART_NUM_2, 50 / portTICK_RATE_MS);
//        sendGPS("PSTMSAVEPAR");
        uart_wait_tx_done(UART_NUM_2, 50 / portTICK_RATE_MS);
    }

    void configMessages() {
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

    void init(bool isWarmStart) {
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



        esp_err_t err;
#if DEFAULT_DATA
        err = getSaveData();
        if(err != ESP_OK){ ESP_LOGE("GPS", "Error writing GPS data: %s", esp_err_to_name(err));}
#endif

        err = getSaveData();
        if(err != ESP_OK){ ESP_LOGE("GPS", "Error reading GPS data: %s", esp_err_to_name(err));}

        xMutex = xSemaphoreCreateMutex();
//    GPS_configMessages();
        if (isWarmStart) { warmStart(); }
        else{coldStart();}

        xTaskCreate(GPSTASK, "GPSLoop", GPS_LOOP_STACK_SIZE, NULL, 5, NULL);
    }

//outputs a string containing specified GPS info
    string send(GPS_send_type_t type) {
        string tosend = "GPS_INFO\n";
        tosend.append(infoStr[STATUS_] + "\n");
        if (type == TIME_DATE || type == ALL) {
            tosend.append("TIME_DATE\n");
            tosend.append(infoStr[TIMESTAMP_] + "\n");
            tosend.append(infoStr[DAY_] + "\n");
            tosend.append(infoStr[MONTH_] + "\n");
            tosend.append(infoStr[YEAR_] + "\n");
        }
        if (type == LOCATION || type == ALL) {
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
        if (type == SPEED_DIR || type == ALL) {
            tosend.append("SPEED_DIR\n");
            tosend.append(infoStr[SPEED_] + "\n");
            tosend.append(infoStr[COURSEMADEGOOD_] + "\n");
            tosend.append(infoStr[MAGVAR_] + "\n");
            tosend.append(infoStr[MAGVARDIR_] + "\n");
        }

        // Wifi_sendTCP((char*) tosend.c_str(), tosend.size());
        return tosend;
    }

    //exports GPS position telemetry for use in UDP packets
    //see Communication protocol document in drive
    string posTelemety(){

        char out[9];
        out[0] = 0x11;

        xSemaphoreTake( xMutex, portMAX_DELAY );
        int tempMinutes = (latMinutes*100000) + latPartMins;
        int tempMinutes2 = ((longDegrees&0x100)<<15) | ((longMinutes*100000) + longPartMins);
        out[1] = *((uint*) &latDegrees);
        out[5] = *((char*) &longDegrees);
        xSemaphoreGive( xMutex );

        out[2] = *(((char*) &tempMinutes)+2);
        out[3] = *(((char*) &tempMinutes)+1);
        out[4] = *(((char*) &tempMinutes));

        out[6] = *(((char*) &tempMinutes2)+2);
        out[7] = *(((char*) &tempMinutes2)+1);
        out[8] = *(((char*) &tempMinutes2));

        return string(out);
    }

    //exports GPS misc telemetry for use in UDP packets
    //see Communication protocol document in drive
    string miscTelemety(){

        char out[6];
        out[0] = 0x12;

        xSemaphoreTake( xMutex, portMAX_DELAY );
        int tempTime = timeStamp;
        uint8_t satStats = (GPSready<<7) + totSats;
        xSemaphoreGive( xMutex );

        out[1] = *(((char*) &tempTime)+3);
        out[2] = *(((char*) &tempTime)+2);
        out[3] = *(((char*) &tempTime)+1);
        out[4] = *(((char*) &tempTime));

        out[5] = satStats;

        return string(out);
    }

//splits given string into lines
//ignores lines that reach eof before newline
//ignores lines that dont start with $
//returns number of generated lines
    int getLines(const uint8_t *in, char **out) {
        int pos = 0;
        for (int i = 0; i < 7; i++) {

            int len = 0;
            if (in[pos] != '$') { return i; }
            for (; in[len + pos] != '\r'; len++) {
                if (in[len + pos] == 0) { return i; }
            }
            out[i] = (char *) malloc(len + 1);

            for (int j = 0; j < len; j++, pos++) {
                out[i][j] = in[pos];
            }
            out[i][len] = 0;
            pos += 2;
        }
        return 7;
    }

//splits cmd into separate strings using commas
//ends when an asterisk is encountered
    int cmdHelper(char *in, char **out) {
        int outPos = 0;
        int outPos2 = 0;
        out[0] = (char *) malloc(40);
        for (int i = 0; in[i] != 0; i++) {
            while (in[i] == ',') {
                out[outPos][outPos2] = 0;
                out[++outPos] = (char *) malloc(40);
                outPos2 = 0;
                i++;
            }
            if (in[i] == '*') {
                out[outPos][outPos2] = 0;
                return outPos;
            }
            out[outPos][outPos2++] = in[i];
        }
        return outPos;
    }


//interprets a cmd and sorts its info into relevant variables
    void cmdParse(char *cmd) {
        char **args = (char **) malloc(sizeof(char *) * 20);
        int numArgs = cmdHelper(cmd, args);
        //NMEA messages
        //except for ones that start with PSTM, those are chip specific
        if (strcmp(args[0], "$GPRMC") == 0) { //position, velocity time
            ESP_LOGI("GPS", "%s\n",cmd);
            if (args[1][0] != 0) {
                infoStr[TIMESTAMP_] = args[1];
                timeStamp = atoi(args[1] + 4);
                args[1][4] = 0;
                args[1][5] = 0;
                timeStamp += 60 * atoi(args[1] + 2);
                args[1][2] = 0;
                args[1][3] = 0;
                timeStamp += 3600 * atoi(args[1]);
            }
            if (args[2][0] != 0) {
                infoStr[STATUS_] = args[2];
                status = args[2][0] == 'A';
            }
            if (args[3][0] != 0) {
                infoStr[LATPARTMINS_] = args[3] + 5;
                latPartMins = atoi(args[3] + 5);
                args[3][4] = 0;
                infoStr[LATMINUTES_] = args[3] + 2;
                latMinutes = atoi(args[3] + 2);
                args[3][2] = 0;
                args[3][3] = 0;
                infoStr[LATDEGREES_] = args[3];
                latDegrees = atoi(args[3]);
            }
            if (args[4][0] != 0) {
                infoStr[NS_] = args[4];
                NS = args[4][0] == 'S';
            }
            if (args[5][0] != 0) {
                infoStr[LONGPARTMINS_] = args[5] + 5;
                longPartMins = atoi(args[5] + 5);
                args[5][4] = 0;
                infoStr[LONGMINUTES_] = args[5] + 2;
                longMinutes = atoi(args[5] + 2);
                args[5][2] = 0;
                args[5][3] = 0;
                infoStr[LONGDEGREES_] = args[5];
                longDegrees = atoi(args[5]);
            }
            if (args[6][0] != 0) {
                infoStr[EW_] = args[6];
                EW = args[6][0] == 'W';
            }
            if (args[7][0] != 0) {
                infoStr[SPEED_] = args[7];
                speed = 10 * atoi(args[7]) + atoi(args[7] + 4);
            }
            if (args[8][0] != 0) {
                infoStr[COURSEMADEGOOD_] = args[8];
                courseMadeGood = 10 * atoi(args[8]) + atoi(args[8] + 4);
            }
            if (args[9][0] != 0) {
                infoStr[YEAR_] = args[9] + 4;
                year = atoi(args[9] + 4);
                args[9][4] = 0;
                infoStr[MONTH_] = args[9] + 2;
                month = atoi(args[9] + 2);
                args[9][2] = 0;
                infoStr[DAY_] = args[9];
                day = atoi(args[9]);
            }
            if (args[10][0] != 0) {
                infoStr[MAGVAR_] = args[10];
                magVar = 10 * atoi(args[10]) + atoi(args[10] + 4);
            }
            if (args[11][0] != 0) {
                infoStr[MAGVARDIR_] = args[11];
                magVarDir = args[11][0] == 'W';
            }
        } else if (strcmp(args[0], "$GPGGA") == 0) { //	Time, position, and fix related data
            ;
        } else if (strcmp(args[0], "$GPVTG") == 0) { //Actual track made good and speed over ground
            ;
        } else if (strcmp(args[0], "$GNGSA") == 0) { //GPS DOP and active satellites
            ;
        } else if (strcmp(args[0]+3, "GSV") == 0) { //GNSS Satellites in View
            ESP_LOGW("GPS", "%s\n",cmd);
            if(args[4][0]!=0){
                totSats = atoi(args[4]);

                GPSready = true;
            }
        } else if (strcmp(args[0], "$GPGLL") == 0) { //	Position data: position fix, time of position fix, and status
            ;
        } else if (strcmp(args[0], "$PSTMCPU") == 0) { //pulse per second data
            ;
        } else {
            ESP_LOGW("GPS", "Invalid command: %s\n",cmd);
        }
        for (int i = 0; i < numArgs; i++) { free(args[i]); }
        free(args);
    }

    void task_Alec(void *ptr) {
        uart_event_t event;
        uint8_t *input = (uint8_t *) malloc(700);
        char *lines[7];
        int inputLen = 0;
        while (1) {
            xSemaphoreTake( xMutex, portMAX_DELAY );
            ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t *) &inputLen));
            inputLen = uart_read_bytes(UART_NUM_2, input, min(inputLen,699), 10 / portTICK_RATE_MS);
            input[min(inputLen,699)] = 0;
            int numLines = getLines(input, lines);
            for (int i = 0; i < numLines; i++) {
                cmdParse(lines[i]);
                free(lines[i]);
            }
            input[0] = 0;
            xSemaphoreGive( xMutex );
            vTaskDelay(250 / portTICK_RATE_MS);
        }
        vTaskDelete(xTaskGetCurrentTaskHandle());
    }


    uint8_t data[512];
    char strbuffer[512];

    void task_Allen(void *arg) {

        int length = 0;

        while (1) {
            ESP_ERROR_CHECK(uart_get_buffered_data_len(uart_num, (size_t *) &length));
            length = uart_read_bytes(uart_num, data, length, 5 / portTICK_RATE_MS);
            printf("%d\n", length);
            int strIndex = 0;
            for (int i = 0; i < length; i++) {
                if (data[i] != '\r') {
                    strbuffer[strIndex++] = data[i];
                    printf("%c", (int8_t) data[i]);
                }
            }
            strbuffer[strIndex++] = '\n';
            strbuffer[strIndex++] = '\n';
            printf("\n-----------------------------------\n");
            Wifi::sendTCP(strbuffer, strIndex - 1);
            vTaskDelay(1000 / portTICK_RATE_MS);
        }

    }

}

