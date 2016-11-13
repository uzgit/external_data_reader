#include "dht.h"
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>

dht DHT;

#define DHT22_PIN 8
#define ONE_WIRE_BUS 4
#define VOLTAGE_PIN 0
#define CURRENT_PIN 1
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);


struct Data
{
  float voltage;
  float current;
  float air_temperature;
  float water_temperature;
  float humidity;
  long  battery_status;
};

Data data;

void setup()
{
  Wire.begin(7);
  Wire.onRequest(WriteEvent);
}

void loop()
{
    // READ DATA
  int chk = DHT.read(DHT22_PIN);
  data.air_temperature = DHT.temperature;
  data.humidity = DHT.humidity;
   
  sensors.requestTemperatures(); // Send the command to get temperatures

  data.water_temperature = sensors.getTempCByIndex(0);
  delay(10);
}

void WriteEvent()
{
  
  byte buffer[24];
  
  insert(data.voltage, buffer,  0);
  insert(data.current, buffer,  4);
  insert(data.air_temperature, buffer,  8);
  insert(data.water_temperature, buffer, 12);
  insert(data.humidity, buffer, 16);
  insert(data.battery_status, buffer, 20);
  
  //write
  Wire.write(buffer, 24);
  
  //Serial.write(buffer, 4);
}

void insert(float to_be_inserted, byte* destination, int offset)
{
  union float2bytes { float f; char b[sizeof(float)]; };
  float2bytes f2b;
  
  f2b.f = to_be_inserted;
  
  for(int i = 0; i < 4; i ++)
  {
    destination[i + offset] = f2b.b[i];
  }
}

void insert(long to_be_inserted, byte* destination, int offset)
{
  union long2bytes { long l; char b[sizeof(float)]; };
  long2bytes l2b;
  
  l2b.l = to_be_inserted;
  
  for(int i = 0; i < 4; i ++)
  {
    destination[i + offset] = l2b.b[i];
  }
}
