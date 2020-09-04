#include "Location.h"
#include "ArduinoJson.h"
#include <Arduino.h>

Location::Location() {
}

Location::Location(String name_, String description, String encodingType, LocationEntry* locationEntry) {
  this->name_ = name_;
  this->description = description;
  this->encodingType = encodingType;
  this->locationEntry = locationEntry;
}

Location::Location(String name_, String description, String encodingType) {
  this->name_ = name_;
  this->description = description;
  this->encodingType = encodingType;
}

void Location::addLocationEntry(LocationEntry* locationEntry) {
  this->locationEntry = locationEntry;
}



void Location::setSelfId(String selfId) {
  this->selfId = selfId;
}

void Location::setThingId(String thingId) {
  this->thingId = thingId;
  linkedToThing = true;
}

void Location::toJSONObject(JsonObject& root) {
  Serial.print("Create JSON");
  StaticJsonBuffer<512> jsonBuffer;
  root["name"] = name_;
  root["description"] = description;
  root["encodingType"] = encodingType;
	JsonObject& locationEntryObject = jsonBuffer.createObject();
  locationEntry->toJSONObject(locationEntryObject);
  root["location"] = locationEntryObject;
  root["@iot.id"] = selfId;
  //root.printTo(jsonString, length_);
}

void Location::toJSONString(char* jsonString, size_t length_) {
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); 
  toJSONObject(root);
 
  root.printTo(jsonString, length_);

  
}

String Location::getSelfId() {
	return selfId;
}
