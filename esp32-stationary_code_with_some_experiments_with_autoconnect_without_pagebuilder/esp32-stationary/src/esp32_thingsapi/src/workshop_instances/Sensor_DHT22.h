#ifndef SENSOR_DHT22_H
#define SENSOR_DHT22_H

#include "../librest/Sensor.h"
#include <Arduino.h>

class DHT22_Sensor : public Sensor
{ 
 private: 
  
 public:
  DHT22_Sensor(String serialNumber);
  
  
}; 

#endif