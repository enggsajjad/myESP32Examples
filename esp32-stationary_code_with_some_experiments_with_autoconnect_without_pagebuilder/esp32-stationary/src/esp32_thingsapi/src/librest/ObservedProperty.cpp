#include "ObservedProperty.h"
#include "ArduinoJson.h"
#include <Arduino.h>


ObservedProperty::ObservedProperty() {

}


ObservedProperty::ObservedProperty(String name_, String description, String definition) {
  this->name_ = name_;
  this->description = description;
  this->definition = definition;
}

void ObservedProperty::setSelfId(String selfId) {
  this->selfId = selfId;
}

String ObservedProperty::getSelfId() {
	return selfId;
}

void ObservedProperty::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Create JSON");
  StaticJsonDocument<512> doc;
  JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts

  root["name"] = name_;
  root["description"] = description;
  root["definition"] = definition;
  root["@iot.id"] = selfId;
  
  serializeJson(doc, jsonString, length_);

  
}
