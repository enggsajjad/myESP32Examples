#include "WorkshopLocationEntry.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

WorkshopLocationEntry::WorkshopLocationEntry(float* coordinates) {
  this->type = "Point";
  this->coordinates[0] = coordinates[0];
  this->coordinates[1] = coordinates[1];
  this->coordinates[2] = coordinates[2];
}



void WorkshopLocationEntry::toJSONObject(JsonObject& root) {
  Serial.print("Create JSON Object");
  root["type"] = type; 
  JsonArray pointArray = root.createNestedArray("coordinates");

  pointArray.add(coordinates[0]);
  pointArray.add(coordinates[1]);
 // if(coordinates[2] != NAN) pointArray->add(coordinates[2]);
  
}

void WorkshopLocationEntry::toJSONString(char* jsonString, size_t length_) {
  StaticJsonDocument<512> doc;
  JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
  toJSONObject(root);
 
  serializeJson(doc, jsonString, length_);

  
}

String WorkshopLocationEntry::getIDRepresentation() {
	
  return String(coordinates[1],precision)+","+String(coordinates[0],precision)+","+String(coordinates[2],precision);

  
}
