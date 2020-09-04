#ifndef CROWDSENSINGNODE_H
#define CROWDSENSINGNODE_H
#include "ArduinoJson.h"
#include <Arduino.h>
#include "../librest/ThingInLocation.h"

class CrowdsensingNode : public ThingInLocation {
 private: 

 public:
  CrowdsensingNode(String serialNumber);
  void toJSONString(char* jsonString, size_t length_);
  
}; 

#endif