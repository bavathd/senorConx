#include"senorConx.h"
#include <Wire.h>
#include <U8g2lib.h>
#include<WiFi.h>
#include <HTTPClient.h>
#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <RTClib.h>


WiFiClient client;

RTC_DS3231 rtc;
String saveData = "";


U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0);
struct _sensorData *sensorData = new _sensorData[30];

senorConx::senorConx(const char* name) {
    if(name == "D") {
        display = true;
    }
    else if(name == "W") {
        wifi = true;
    }
    else if(name="D+W") {
        display = true;
        wifi = true;
    }
    else {}
}

void senorConx::initialize(void* (*func) ()) {
  
    func();

    if(display) displayStart();
    if(wifi) wifiStart();
    if (! rtc.begin()) {

    Serial.println("Couldn't find RTC");
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
    u8g2.drawStr(3, 15, "RTC Failed");
    u8g2.sendBuffer();
    delay(1000);
    Serial.flush();
    while (1) delay(10);
  }

    if(!SD.begin(D2)){
        Serial.println("Card Mount Failed");
        u8g2.clearBuffer();         // clear the internal memory
        u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
        u8g2.drawStr(3, 15, "SD Failed");
        u8g2.sendBuffer();
        delay(1000);
        return;
    }

   
    
}

void senorConx::setTime(int day, int month, int year, int H, int M, int S) {
    rtc.adjust(DateTime(year, month, day, H, M, S));
}


  template <typename T>
    void senorConx::setPayload(const char* name, const T& value, uint16_t index) {
        sensorData[index].name = name;
        sensorData[index].value = toString(value);
        if (index > maxindex) {
            maxindex = index;
        }
    }

    template <>
    void senorConx::setPayload<unsigned short>(const char* name, const unsigned short& value, uint16_t index)
    {
        sensorData[index].name = name;
        sensorData[index].value = String(value);
        if (index > maxindex) {
            maxindex = index;
        }
    }
    template <>
    void senorConx::setPayload<char [4]>(const char* name, const char(&value) [4], uint16_t index)
    {
        sensorData[index].name = name;
        sensorData[index].value = String(value);
        if (index > maxindex) {
            maxindex = index;
        }
    }
    template <>
    void senorConx::setPayload<unsigned int>(const char* name, const unsigned int& value, uint16_t index)
    {
        sensorData[index].name = name;
        sensorData[index].value = String(value);
        if (index > maxindex) {
            maxindex = index;
        }
    }


    template <>
    void senorConx::setPayload<float>(const char* name, const float& value, uint16_t index)
    {
        sensorData[index].name = name;
        sensorData[index].value = String(value);
        if (index > maxindex) {
            maxindex = index;
        }
    }

    template <>
    void senorConx::setPayload<int>(const char* name, const int& value, uint16_t index)
    {
        sensorData[index].name = name;
        sensorData[index].value = String(value);
        if (index > maxindex) {
            maxindex = index;
        }
    }

    template <typename T>
    String senorConx::toString(const T& value) {
        return String(value);
    }
    String senorConx::toString(const char* value) {
       return String(value);
    }


void senorConx::looper(void*(*func)())
{
  
    func();

    send();
}

void senorConx::send()
{   uint16_t n = 0;
    saveData = "";
    String time = readTime();
    saveData += time;
    while(n <= maxindex)
    {
        String data[2];
        for(int i=0; i<2; i++) 
        {   
            if(n+i > maxindex) {
                data[i] = "\0";
                break;
            }
            data[i] = String(sensorData[n+i].name) + ": " + sensorData[n+i].value; 
        }
        saveData = ","+ data[0] + "," + data[1];
        appendFile(SD,"/datalog.txt", saveData.c_str());
        if(display)
        {
            
            u8g2.clearBuffer();         // clear the internal memory
            u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
            u8g2.drawStr(4,10,time.c_str());
            u8g2.drawStr(0,35,data[0].c_str());
            u8g2.drawStr(0,50,data[1].c_str());
            Serial.println(data[0]);
            Serial.println(data[1]);
            u8g2.sendBuffer();
            if(!wifi)
            {
                delay(1500);
            }	
        }
        else {
            Serial.println(data[0]);
            Serial.println(data[1]);
        } 
        
        if(wifi) 
        {
            bool sendCheck1 = sendData(sensorData[n].name, sensorData[n].value);
            n++;

            if(n >= maxindex){
                
                if(sendCheck1) {
                            // clear the internal memory
                   
                    if(display)
                    {
                        u8g2.clearBuffer(); 
                        u8g2.setFont(u8g2_font_ncenB10_tr);
                        u8g2.drawStr(0,35,"Data sent ");
                        Serial.println("Data sent ");
                        u8g2.sendBuffer();	           
                        
                    }
                    else {
                        Serial.println("Data sent ");
                    
                    } 
                    delay(1500);
                }
                else {
                    if(display)
                    {
                        u8g2.clearBuffer(); 
                        u8g2.setFont(u8g2_font_ncenB10_tr);
                        u8g2.drawStr(0,35,"Data Failed");
                        Serial.println("Data Failed");
                        u8g2.sendBuffer();	           
                        
                    }
                    else {
                        Serial.println("Data Failed");
                    
                    } 
                    delay(1500);
                }
                break;
            }

            bool sendCheck2 = sendData(sensorData[n].name, sensorData[n].value);


            u8g2.clearBuffer();         // clear the internal memory
            u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
            
            if(sendCheck1 && sendCheck2) {
                if(display)
                {
                    u8g2.clearBuffer();         // clear the internal memory
                    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
                    u8g2.drawStr(0,35,"Data sent ");
                    Serial.println("Data sent ");
                    u8g2.sendBuffer();	           
                    
                }
                else {
                    Serial.println("Data sent ");
                
                } 
                delay(1500);
            }
            else {
                if(display)
                {   u8g2.clearBuffer();         
                    u8g2.setFont(u8g2_font_ncenB10_tr);
                    u8g2.drawStr(0,35,"Data Failed");
                    Serial.println("Data Failed");
                    u8g2.sendBuffer();	           
                    
                }
                else {
                    Serial.println("Data Failed");
                
                } 
                delay(1500);
            }
        }
        else {
            n++;
        }
        n++;
    }

}



void senorConx::displayStart()
{   u8g2.setBusClock(100000);
    if(!u8g2.begin())
    {
        Serial.println("Display Failed");
        while(1);
    }
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
    u8g2.drawStr(3, 15, "Initializing");
    u8g2.sendBuffer();
    delay(1000);
}

void senorConx::wifiStart() {
    Serial.println("Starting wifi");
    if (display) {
        u8g2.clearBuffer();         // clear the internal memory
        u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
        u8g2.drawStr(3, 15, "Connecting to");
        u8g2.drawStr(3,35, String(ssid).c_str());

        u8g2.sendBuffer();
    }


    if(!wifiConnect()) {
        if (display) 
        {
            u8g2.clearBuffer();         // clear the internal memory
            u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
            u8g2.drawStr(3, 15, "wifi Failed to connect");
            u8g2.sendBuffer();
        }
        Serial.println("wifi not connected");
        while(1);
    }
    wifi = true;


    if (display) 
    {
        u8g2.clearBuffer();         // clear the internal memory
        u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
        u8g2.drawStr(3, 15, "WiFi connected");
        u8g2.sendBuffer();
    }
        Serial.println("Wifi Connected");
        delay(1000);
    
}

bool senorConx::wifiConnect() {
    WiFi.begin(ssid, pass);
    
    delay(3000);
    Serial.println("\nConnecting");
    
    int timeout_counter = 0;

    while(true)
    {
        if(WiFi.status() == WL_CONNECTED) {
            break;
        }
        Serial.print(".");
        delay(500);
        timeout_counter++;
        if(timeout_counter >= 10*2)
        {
            Serial.println("\nConnection Timeout");
            return false;
        }
    
    }
    return true;
}


bool senorConx::sendData(const char* variables, String values)
{
    Serial.println("\nSending Data");

        String PostData = String("{\"variable\":") + String("\"") + String(variables) + String("\"") + String(",\"value\":") + String(values) + String("}");
        String Dev_token = String("Device-Token: ")+ token;
        Serial.println(variables);
        Serial.println( values);
        if (client.connect(tagoServer,80)) 
        {                     
        Serial.println("connected to server");
        // Make a HTTP request:
        client.println("POST /data? HTTP/1.1");
        client.println("Host: api.tago.io");
        client.println("_ssl: false");       // for non-secured connection, use this option "_ssl: false"
        client.println(Dev_token);
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(PostData.length());
        client.println();
        client.println(PostData);
        delay(1500);
        return true;
        }
        else 
        {
        // if you couldn't make a connection:
        Serial.println("connection failed");
        delay(3000);
        return false;
        }
        
   
}


senorConx::~senorConx() {
    client.stop();
    Serial.println("\nDisconnecting API");
    WiFi.disconnect();
    Serial.println("\nDisconnected Wifi");
    Serial.flush();
}

void senorConx::readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void senorConx::writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
        u8g2.clearBuffer();         // clear the internal memory
        u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
        u8g2.drawStr(3, 15, "sd saved");
        u8g2.sendBuffer();
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void senorConx::appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
        u8g2.clearBuffer();         // clear the internal memory
        u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
        u8g2.drawStr(3, 15, "sd saved");
        u8g2.sendBuffer();
        delay(250);
    } else {
        Serial.println("Append failed");
        u8g2.clearBuffer();         // clear the internal memory
        u8g2.setFont(u8g2_font_ncenB10_tr); // choose a suitable font  
        u8g2.drawStr(3, 15, "sd Failed");
        u8g2.sendBuffer();
        delay(1000);
    }
    file.close();
}

String senorConx::readTime()
{
DateTime now = rtc.now();
Serial.print(now.year(), DEC);
Serial.print('/');
Serial.print(now.month(), DEC);
Serial.print('/');
Serial.print(now.day(), DEC);
Serial.print("\t");
Serial.print(now.hour(), DEC);
Serial.print(':');
Serial.print(now.minute(), DEC);
Serial.print(':');
Serial.print(now.second(), DEC);
Serial.println();
return String(now.day()) + "/" + String(now.month())+ "/" + String(now.year()) + "," + String(now.hour())+ ":" + String(now.minute()) + ":" + String(now.second());
 
}
