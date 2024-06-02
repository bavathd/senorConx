#include <senorConx.h>
#include "DHT.h"

#define DHTPIN D6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
senorConx connc("D+W");

void setup() {
    connc.ssid = "Quest_Lab_Weather";
    connc.pass = "Questlab123";
    connc.token = "17876494-9075-4547-84f8-68c12f7ddee1";
    connc.initialize((void*(*)())start);
    
}

void loop()
{
    connc.looper((void*(*)())repeat);
}

void start() {
 Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();

}

void repeat()
{
 delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
connc.setPayload("Humidity",dht.readHumidity(),0);                                 
  // Read temperature as Celsius (the default)
connc.setPayload("Temp(°C)",dht.readTemperature(),1);
connc.setPayload("Temp(°F)",dht.readTemperature(true),2);
  // Read temperature as Fahrenheit (isFahrenheit = true)


  // Check if any reads failed and exit early (to try again).
  if (isnan(dht.readHumidity()) || isnan(dht.readTemperature())) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


 
}
