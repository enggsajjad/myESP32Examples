#ifndef SENSOR_BME280_H
#define SENSOR_BME280_H

#include "../librest/Sensor.h"
#include <Arduino.h>

class BME280_Sensor : public Sensor
{ 
 private: 
  
 public:
  BME280_Sensor(String serialNumber);
  
  
}; 

#endif