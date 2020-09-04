#ifndef EXTENDED_THING_H
#define EXTENDED_THING_H

#include <Arduino.h>
#include "../librest/Thing.h"

class ExtendedThing : public Thing
{ 

 protected: 
	ToJSONObject* property;
	bool propertySet;
	
 public:
  ExtendedThing();  
  ExtendedThing(String name_, String description, String selfId);
  ExtendedThing(String name_, String description, String serialNumber, String selfId);
  virtual void setProperty(ToJSONObject* property);
  virtual void toJSONObject(JsonObject& root);
  virtual void toJSONString(char* jsonString, size_t length_);
  
}; 

#endif