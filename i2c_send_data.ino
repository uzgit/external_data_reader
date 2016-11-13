#include <Wire.h>
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
  data.voltage = 10;
  data.current = 1;
  data.air_temperature = 0;
  data.water_temperature = 25;
  data.humidity = 50;
  data.battery_status = 0;
  Wire.begin(7);
  Wire.onRequest(WriteEvent);
  
  Serial.begin(9600);
}

void loop()
{
  data.voltage++;
  data.current = data.voltage / 5.0;
  data.air_temperature ++;
  data.water_temperature ++;
  data.humidity ++;
  data.battery_status = 0;
  
  delay(300);
}

//void WriteEvent()
//{
//  Serial.println("Transfer request made via i2c.");
//}

void WriteEvent()
{
  Serial.println("Transfer request made via i2c.");
  
  byte buffer[24];
  
  insert(data.voltage, buffer,  0);
  insert(data.current, buffer,  4);
  insert(data.air_temperature, buffer,  8);
  insert(data.water_temperature, buffer, 12);
  insert(data.humidity, buffer, 16);
  insert(data.battery_status, buffer, 20);
  
  //write
  Wire.write(buffer, 24);
  
  Serial.write(buffer, 4);
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
