#ifndef WORKSHOPLOCATION_H
#define WORKSHOPLOCATION_H
#include "ArduinoJson.h"
#include <Arduino.h>
#include "../librest/Location.h"

class WorkshopLocation: public Location
{ 
 protected: 
 public:
  WorkshopLocation(String humanReadableLocation, LocationEntry* locationEntry);
 
  
}; 

#endif
