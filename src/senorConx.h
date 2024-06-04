#ifndef __SENORCONX_H__
#define __SENORCONX_H_

#include<Arduino.h>





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
    char* tagoServer = "api.tago.io";
};
#endif