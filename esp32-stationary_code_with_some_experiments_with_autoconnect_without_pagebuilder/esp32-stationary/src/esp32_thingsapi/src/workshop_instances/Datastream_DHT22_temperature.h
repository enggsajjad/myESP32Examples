#ifndef DATASTREAM_DHT22_TEMPERATURE_H
#define DATASTREAM_DHT22_TEMPERATURE_H

#include "../librest/Datastream.h"
#include "ExtendedDatastream.h"
#include <Arduino.h>

class DatastreamTemperatureDHT22 : public ExtendedDatastream
{ 
 private: 
  
 public:
	DatastreamTemperatureDHT22(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty);
  DatastreamTemperatureDHT22(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId);
  
  
}; 

#endif