#ifndef DATASTREAM_BME280_TEMPERATURE_H
#define DATASTREAM_BME280_TEMPERATURE_H

#include "../librest/Datastream.h"
#include "ExtendedDatastream.h"
#include <Arduino.h>

class DatastreamTemperatureBME280 : public ExtendedDatastream
{ 
 private: 
  
 public:
	DatastreamTemperatureBME280(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty);
  DatastreamTemperatureBME280(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId);
  
  
}; 

#endif