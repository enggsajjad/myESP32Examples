#ifndef SENSOR_SDS011_H
#define SENSOR_SDS011_H

#include "../librest/Sensor.h"
#include <Arduino.h>

class SDS011_Sensor : public Sensor
{ 
 private: 
  
 public:
  SDS011_Sensor(String serialNumber);
  
  
}; 

#endif