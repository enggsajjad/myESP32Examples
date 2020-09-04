#ifndef TO_JSON_OBJECT_H
#define TO_JSON_OBJECT_H

#include "ArduinoJson.h"
#include <Arduino.h>


class ToJSONObject
{ 
 public: 
	virtual void toJSONObject(JsonObject& root) = 0;

  
}; 

#endif