#ifndef TO_JSON_STRING_H
#define TO_JSON_STRING_H

#include "ArduinoJson.h"
#include <Arduino.h>

class ToJSONString 
{ 
 public: 
	virtual void toJSONString(char* jsonString, size_t length_) = 0;


  
}; 

#endif