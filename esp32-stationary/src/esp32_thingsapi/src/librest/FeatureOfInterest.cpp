#include "FeatureOfInterest.h"
#include "ArduinoJson.h"
#include <Arduino.h>

FeatureOfInterest::FeatureOfInterest(String name_, String description, String encodingType) {
  this->name_ = name_;
  this->description = description;
  this->encodingType = encodingType;
}

void FeatureOfInterest::setSelfId(String selfId) {
  this->selfId = selfId;
}

void FeatureOfInterest::setFeature(String type, float* coordinates) {
  this->feature.type = type;
  this->feature.coordinates[0] = coordinates[0];
  this->feature.coordinates[1] = coordinates[1];
}

void FeatureOfInterest::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Create JSON");
  StaticJsonBuffer<400> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); //root object filled with further json objects

  root["name"] = name_;
  root["description"] = description;
  root["encodingType"] = encodingType;

  JsonObject& featureObject = jsonBuffer.createObject();
  feature.toJSONObject(featureObject);
  root["feature"] = featureObject;
  
  root["@iot.id"] = selfId;
  root.printTo(jsonString, length_);
}

void FeatureOfInterest::Feature::toJSONObject(JsonObject& jsonObject) {
  Serial.print("Create inner JSON");
  jsonObject["type"] = this->type;

  //array in neuem puffer zu erzeugen könnte kritisch sein, weil puffer nach
  //verlassen der funktion gelöscht wird, ausprobieren
  StaticJsonBuffer<100> jsonBuffer;
  JsonArray* pointArray = NULL;
  pointArray = &jsonBuffer.createArray();
  Serial.print("Coordinates[0] "+String(coordinates[0]));
  Serial.print("Coordinates[1] "+String(coordinates[1]));
  pointArray->add(coordinates[0]);
  pointArray->add(coordinates[1]);
  
  jsonObject["coordinates"] = *pointArray;
  
}



