#include "ThingInLocation.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ThingInLocation::ThingInLocation() {
  
}

ThingInLocation::ThingInLocation(String name_, String description, String serialNumber) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
}

ThingInLocation::ThingInLocation(String name_, String description, String serialNumber, String selfId) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
  this->selfId = selfId;
}

void ThingInLocation::addLocation(Location* location) {
  this->location = location;
}



//location not yet implemented
void ThingInLocation::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Create JSON");
  StaticJsonBuffer<2048> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); //root object filled with further json obejcts
  
  root["name"] = name_;
  root["description"] = description;
  root["@iot.id"] = selfId;

  
  JsonObject& locationObject = jsonBuffer.createObject();
  this->location->toJSONObject(locationObject);
  


  JsonArray* locationArray = NULL;
  locationArray = &jsonBuffer.createArray();
  locationArray->add(locationObject);

  root["Locations"] = *locationArray;
  
  root.printTo(jsonString, length_);
}

  


