#include "CrowdsensingNode.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>


CrowdsensingNode::CrowdsensingNode(String serialNumber) {
  this->serialNumber = serialNumber;
  this->name_ = "Crowdsensing Node (SDS011, "+serialNumber+")";
  this->description = "Low Cost Node Measuring Particulate Matter";
	String tempId = "teco.edu:crowdsensor:2019-03:"+serialNumber;
	Serial.println("tempId: "+ tempId);
	this->selfId = FROSTEntities::Thing::preambula+":"+toHEXSHA1(tempId).substring(0,7);
  
}


void CrowdsensingNode::toJSONString(char* jsonString, size_t length_) {
  //use own implementation, if base class implementation isn't adequate
  
  ExtendedThingInLocation::toJSONString(jsonString, length_);
}

  


