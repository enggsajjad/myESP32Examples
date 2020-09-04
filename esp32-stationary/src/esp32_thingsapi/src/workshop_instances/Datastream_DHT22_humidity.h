#ifndef DATASTREAM_DHT22_HUMIDITY_H
#define DATASTREAM_DHT22_HUMIDITY_H

#include "../librest/Datastream.h"
#include "ExtendedDatastream.h"
#include <Arduino.h>

class DatastreamHumidityDHT22 : public ExtendedDatastream
{ 
 private: 
  
 public:
	DatastreamHumidityDHT22(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty);
  DatastreamHumidityDHT22(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId);
  
  
}; 

#endif