#ifndef CROWDSENSINGNODE_H
#define CROWDSENSINGNODE_H
#include "ArduinoJson.h"
#include <Arduino.h>
#include "ExtendedThingInLocation.h"

class CrowdsensingNode : public ExtendedThingInLocation {
 private: 

 public:
  CrowdsensingNode(String serialNumber);
  void toJSONString(char* jsonString, size_t length_);
  
}; 

#endif