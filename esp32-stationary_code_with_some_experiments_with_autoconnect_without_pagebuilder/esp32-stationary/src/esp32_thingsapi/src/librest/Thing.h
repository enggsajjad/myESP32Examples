#ifndef THING
#define THING

#include "ToJSONString.h"
#include "ToJSONObject.h"
#include <Arduino.h>

class Thing : public ToJSONString, public ToJSONObject
{ 
 protected: 
  String name_; 
  String description; 
  String locationId;
  String serialNumber;
  String selfId; 
  bool locationIdSet = false;
 public:
  Thing();  
  Thing(String name_, String description, String selfId);
  Thing(String name_, String description, String serialNumber, String selfId);
  void setLocationId(String locationId);
  virtual void toJSONString(char* jsonString, size_t length_);
  virtual void toJSONObject(JsonObject& root);
	String getSelfId();
	String getSerialNumber();
  
}; 

#endif