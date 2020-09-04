#ifndef License_Property_H
#define License_Property_H

#include "ArduinoJson.h"
#include <Arduino.h>
#include "../librest/ToJSONString.h"
#include "../librest/ToJSONObject.h"

//Property in Classname indicates, that it shell be only used in the property section of an element

class LicenseProperty: public ToJSONObject, public ToJSONString
{ 
 private:
 	String type;
	String owner;
	String metadata;
 public: 
 	LicenseProperty();
	LicenseProperty(String type, String owner, String metadata);
	void toJSONObject(JsonObject& root);
	void toJSONString(char* jsonString, size_t length_);
  
}; 

#endif