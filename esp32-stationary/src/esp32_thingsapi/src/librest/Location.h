#ifndef LOCATION
#define LOCATION
#include "ToJSONString.h"
#include "ArduinoJson.h"
#include <Arduino.h>
#include "LocationEntry.h"

class Location : public ToJSONString
{ 
 protected: 
  String name_;
  String description;
  String encodingType;
  String selfId;
  String thingId;
  LocationEntry* locationEntry;
  bool linkedToThing = false;
 public:
  Location(String name_, String description, String encodingType, LocationEntry* locationEntry);
  Location(String name_, String description, String encodingType);
  Location();
  void addLocationEntry(LocationEntry* locationEntry);
  void setSelfId(String selfId);
  void setThingId(String thingId);
  void toJSONObject(JsonObject& root);
  void toJSONString(char* jsonString, size_t length_);
  String getSelfId();
  
}; 

#endif
