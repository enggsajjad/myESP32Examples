#include "Sensor.h"
#include "ArduinoJson.h"
#include <Arduino.h>

Sensor::Sensor() {
}

Sensor::Sensor(String name_, String description, String encodingType, String metadata, String serialNumber) {
  this->name_ = name_;
  this->description = description;
  this->encodingType = encodingType;
  this->metadata = metadata;
	this->serialNumber = serialNumber;
}

void Sensor::setSelfId(String selfId) {
  this->selfId = selfId;
}

String Sensor::getSelfId() {
  return selfId;
}

String Sensor::getSerialNumber() {
  return serialNumber;
}

void Sensor::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Create JSON");
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); //root object filled with further json obejcts

  root["name"] = name_;
  root["description"] = description;
  root["encodingType"] = encodingType;
  root["metadata"] = metadata;
  root["@iot.id"] = selfId;
  root.printTo(jsonString, length_);

  
}
