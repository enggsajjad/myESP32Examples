#ifndef DATASTREAM_BME280_PRESSURE_H
#define DATASTREAM_BME280_PRESSURE_H

#include "../librest/Datastream.h"
#include "ExtendedDatastream.h"
#include <Arduino.h>

class DatastreamPressureBME280 : public ExtendedDatastream
{ 
 private: 
  
 public:
	DatastreamPressureBME280(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty);
  DatastreamPressureBME280(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId);
  
  
}; 

#endif