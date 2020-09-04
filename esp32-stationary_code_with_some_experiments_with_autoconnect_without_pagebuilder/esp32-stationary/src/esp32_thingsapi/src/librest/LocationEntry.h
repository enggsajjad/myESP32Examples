#ifndef LOCATION_ENTRY_H
#define LOCATION_ENTRY_H

#include "ArduinoJson.h"
#include <Arduino.h>

class LocationEntry 
{ 
 public: 
	virtual void toJSONObject(JsonObject& root) = 0;
	virtual String getIDRepresentation() = 0;	
	//virtual LocationEntry();
	//virtual ~LocationEntry();
  
}; 

#endif