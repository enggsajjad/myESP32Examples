#ifndef OBSERVED_PROPERTY
#define OBSERVED_PROPERTY

#include "ToJSONString.h"
#include <Arduino.h>

class ObservedProperty : public ToJSONString
{ 
 protected: 
  String name_;
  String description;
  String definition;
 
  String selfId;
  
 public:
	ObservedProperty();
  ObservedProperty(String name_, String description, String definition);
  void setSelfId(String selfId);
	String getSelfId();
  void toJSONString(char* jsonString, size_t length_);
  
}; 

#endif
