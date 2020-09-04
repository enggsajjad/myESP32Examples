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
  StaticJsonBuffer<256> jsonBuffer;
 
  JsonArray* pointArray = NULL;
  pointArray = &jsonBuffer.createArray();
  pointArray->add(coordinates[0]);
  pointArray->add(coordinates[1]);
 // if(coordinates[2] != NAN) pointArray->add(coordinates[2]);

  root["type"] = type; 
  root["coordinates"] = *pointArray;
  
}

void WorkshopLocationEntry::toJSONString(char* jsonString, size_t length_) {
  StaticJsonBuffer<512> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); 
  toJSONObject(root);
 
  root.printTo(jsonString, length_);

  
}

String WorkshopLocationEntry::getIDRepresentation() {
	
  return String(coordinates[1],precision)+","+String(coordinates[0],precision)+","+String(coordinates[2],precision);

  
}
