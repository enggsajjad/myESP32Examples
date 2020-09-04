#ifndef Firmware_Property_H
#define Firmware_Property_H

#include "ArduinoJson.h"
#include <Arduino.h>
#include "../librest/ToJSONString.h"
#include "../librest/ToJSONObject.h"

//Property in Classname indicates, that it shell be only used in the property section of an element

class FirmwareProperty: public ToJSONObject, public ToJSONString
{ 
 private:
 	String date;
	String version;
 public: 
 	FirmwareProperty();
	FirmwareProperty(String date, String version);
	void toJSONObject(JsonObject& root);
	void toJSONString(char* jsonString, size_t length_);
  
}; 

#endif