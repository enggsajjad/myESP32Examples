#include "ExtendedThingInLocation.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ExtendedThingInLocation::ExtendedThingInLocation() {
  
}

ExtendedThingInLocation::ExtendedThingInLocation(String name_, String description, String serialNumber) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
}

ExtendedThingInLocation::ExtendedThingInLocation(String name_, String description, String serialNumber, String selfId) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
  this->selfId = selfId;
}

void ExtendedThingInLocation::addLocation(Location* location) {
  this->location = location;
}


void ExtendedThingInLocation::toJSONObject(JsonObject& root) {
 Serial.print("ExtendedThingInLocation: Create JSONObject");
 
  
  ExtendedThing::toJSONObject(root);  //uses ExtendedThing method for creation of the base json entries

  JsonArray locationArray = root.createNestedArray("Locations");
  JsonObject locationObject = locationArray.createNestedObject();
	
  this->location->toJSONObject(locationObject);
  
}

//location not yet implemented
void ExtendedThingInLocation::toJSONString(char* jsonString, size_t length_) {
 Serial.print("ExtendedThingInLocation: Create JSON String");
  StaticJsonDocument<2048> doc;
  JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
  toJSONObject(root);
  serializeJson(doc, jsonString, length_);
}




  


