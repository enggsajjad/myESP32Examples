#ifndef EXTENDED_THING_IN_LOCATION_H
#define EXTENDED_THING_IN_LOCATION_H

#include <Arduino.h>
#include "ExtendedThing.h"
#include "../librest/Location.h"

class ExtendedThingInLocation : public ExtendedThing
{ 
 protected: 
  Location* location = NULL;
 public:
  ExtendedThingInLocation();  
  ExtendedThingInLocation(String name_, String description, String selfId);
  ExtendedThingInLocation(String name_, String description, String serialNumber, String selfId);
  void toJSONString(char* jsonString, size_t length_);
  void toJSONObject(JsonObject& root);
  void addLocation(Location* location);
  
}; 

#endif