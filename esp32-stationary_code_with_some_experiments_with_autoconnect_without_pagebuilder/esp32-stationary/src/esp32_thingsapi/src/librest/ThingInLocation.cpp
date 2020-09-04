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


void ThingInLocation::toJSONObject(JsonObject& root) {
 Serial.print("ThingInLocation: Create JSONObject");
  Thing::toJSONObject(root);  //uses things method for creation of the base json entries
	
  JsonArray locationArray = root.createNestedArray("Locations");
  JsonObject locationObject = locationArray.createNestedObject();
	
  this->location->toJSONObject(locationObject);
  
  
}

//location not yet implemented
void ThingInLocation::toJSONString(char* jsonString, size_t length_) {
 Serial.print("ThingInLocation: Create JSON String");
  StaticJsonDocument<2048> doc;
  JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
  toJSONObject(root);
  serializeJson(doc, jsonString, length_);
}




  


