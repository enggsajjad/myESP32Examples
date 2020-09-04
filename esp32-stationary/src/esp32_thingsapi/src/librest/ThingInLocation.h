#ifndef THINGINLOCATION_H
#define THINGINLOCATION_H

#include <Arduino.h>
#include "Thing.h"
#include "Location.h"

class ThingInLocation : public Thing
{ 
 protected: 
  Location* location = NULL;
 public:
  ThingInLocation();  
  ThingInLocation(String name_, String description, String selfId);
  ThingInLocation(String name_, String description, String serialNumber, String selfId);
  void toJSONString(char* jsonString, size_t length_);
  void addLocation(Location* location);
  
}; 

#endif