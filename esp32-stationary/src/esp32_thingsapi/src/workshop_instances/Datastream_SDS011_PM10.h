#ifndef DATASTREAM_SDS011_PM10_H
#define DATASTREAM_SDS011_PM10_H

#include "../librest/Datastream.h"
#include "ExtendedDatastream.h"
#include <Arduino.h>

class Datastream_PM10 : public ExtendedDatastream
{ 
 private: 
  
 public:
	Datastream_PM10(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty);
  Datastream_PM10(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId);
  
  
}; 

#endif