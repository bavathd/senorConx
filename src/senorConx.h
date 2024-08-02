#ifndef __SENORCONX_H__
#define __SENORCONX_H_

#include<Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include<WiFi.h>
#include <HTTPClient.h>
// #include "FS.h"
// #include "SD.h"
// #include <SPI.h>
// #include <RTClib.h>




using namespace std;

struct _sensorData{
    const char* name;
    String value;
};

class senorConx {
public:
    senorConx(const char* mode);
    ~senorConx();
    const char* ssid;
    const char* pass;
    const char* token;
    void initialize(void* (func) ());
    template <typename T>
    void setPayload(const char* name, const T& value, uint16_t index); 
    void looper(void*(func)());
  
private:

    void displayStart();
    void wifiStart();


    // void readFile(fs::FS &fs, const char * path);
    // void writeFile(fs::FS &fs, const char * path, const char * message);
    // void appendFile(fs::FS &fs, const char * path, const char * message);

    // String readTime();
    // void setTime(int day, int month, int year, int H, int M, int S);


    template <typename T>
    String toString(const T& value);
    String toString(const char* value);
    uint16_t maxindex=0;
    bool wifiConnect();
    bool display = false;
    bool wifi = false;
    void send();
    bool sendData(const char* variables, String values);
    int keyindex = 0;
    const char* tagoServer = "api.tago.io";
};
#endif