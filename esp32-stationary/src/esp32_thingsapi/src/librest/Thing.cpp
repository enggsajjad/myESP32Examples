#include "Thing.h"
#include "ArduinoJson.h"
#include <Arduino.h>

Thing::Thing() {
  
}
Thing::Thing(String name_, String description, String serialNumber) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
}

Thing::Thing(String name_, String description, String serialNumber, String selfId) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
  this->selfId = selfId;
}



void Thing::setLocationId(String locationId) {
  this->locationId = locationId;
  locationIdSet = true;
  
}

//location not yet implemented
void Thing::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Create JSON");
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); //root object filled with further json obejcts
  //alle bezeichner zentral verwalten in einem header
  root["name"] = name_;
  root["description"] = description;
  root["@iot.id"] = selfId;

  if(locationIdSet) {
    JsonObject& locationObject = jsonBuffer.createObject();
    locationObject["@iot.id"] = locationId;
  
    JsonArray* locationArray = NULL;
    locationArray = &jsonBuffer.createArray();
    locationArray->add(locationObject);
    
    root["Locations"] = *locationArray;
  }
  root.printTo(jsonString, length_);
}

String Thing::getSelfId() {
	return selfId;
}

String Thing::getSerialNumber() {
	return serialNumber;
}
  


