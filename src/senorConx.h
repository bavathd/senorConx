#ifndef __SENORCONX_H__
#define __SENORCONX_H_

#include<Arduino.h>
#include<driver/i2c.h>
#include<esp_rom_gpio.h>

#define I2C_SDA1 D4 
#define I2C_SCL1 D5
#define I2C_SDA2 D9 
#define I2C_SCL2 D10


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
    void activate_I2C_GPIO_a();
    void activate_I2C_GPIO_b();
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