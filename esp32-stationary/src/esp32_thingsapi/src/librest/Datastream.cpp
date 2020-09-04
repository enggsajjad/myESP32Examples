#include "Datastream.h"
#include "ArduinoJson.h"
#include <Arduino.h>

Datastream::Datastream() {
 
}

Datastream::Datastream(String name_, String description, String observationType) {
  this->name_ = name_;
  this->description = description;
  this->observationType = observationType;
  
}


void Datastream::setSelfId(String selfId) {
  this->selfId = selfId;
}

void Datastream::setSensorId(String sensorId) {
  this->sensorId = sensorId;
}

void Datastream::setThingId(String thingId) {
  this->thingId = thingId;
}

void Datastream::setObservedPropertyId(String observedPropertyId) {
  this->observedPropertyId = observedPropertyId;
}

void Datastream::toJSONObject(JsonObject& root) { //old function for maintaining compatibility 
	StaticJsonBuffer<2048> jsonBuffer; //inner buffer shouldnt exceed outer buffer
	//alle bezeichner zentral verwalten in einem header
	root["name"] = name_;
	root["description"] = description;
	root["observationType"] = observationType;

	JsonObject& unitOfMeasObject = jsonBuffer.createObject();
	unitOfMeasurement.toJSONObject(unitOfMeasObject);

	root["unitOfMeasurement"] = unitOfMeasObject;

	JsonObject& thingsIdObject = jsonBuffer.createObject();
	thingsIdObject["@iot.id"] = thingId;

	JsonObject& sensorIdObject = jsonBuffer.createObject();
	sensorIdObject["@iot.id"] = sensorId;

	JsonObject& observedPropertyIdObject = jsonBuffer.createObject();
	observedPropertyIdObject["@iot.id"] = observedPropertyId;

	root["Thing"] = thingsIdObject;
	root["Sensor"] = sensorIdObject;
	root["ObservedProperty"] = observedPropertyIdObject;

	root["@iot.id"] = selfId;
}


/*void Datastream::toJSONObject(JsonObject& root, StaticJsonBuffer<2048> &jsonBuffer) {
	Serial.print("Datastream: Create JSON Object");
	//StaticJsonBuffer<2048> jsonBuffer; //inner buffer shouldnt exceed outer buffer
	//alle bezeichner zentral verwalten in einem header
	root["name"] = name_;
	root["description"] = description;
	root["observationType"] = observationType;

	JsonObject& unitOfMeasObject = jsonBuffer.createObject();
	unitOfMeasurement.toJSONObject(unitOfMeasObject);

	root["unitOfMeasurement"] = unitOfMeasObject;

	JsonObject& thingsIdObject = jsonBuffer.createObject();
	thingsIdObject["@iot.id"] = thingId;

	JsonObject& sensorIdObject = jsonBuffer.createObject();
	sensorIdObject["@iot.id"] = sensorId;

	JsonObject& observedPropertyIdObject = jsonBuffer.createObject();
	observedPropertyIdObject["@iot.id"] = observedPropertyId;

	root["Thing"] = thingsIdObject;
	root["Sensor"] = sensorIdObject;
	root["ObservedProperty"] = observedPropertyIdObject;

	root["@iot.id"] = selfId;
	
}*/

void Datastream::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Datastream: Create JSON String");
  StaticJsonBuffer<2048> jsonBuffer;
  //StaticJsonBuffer<2048>* jsonBufferPointer = &jsonBuffer; 	
  JsonObject& root = jsonBuffer.createObject(); //root object filled with further json obejcts
  toJSONObject(root);
  
  root.printTo(jsonString, length_);
}

void Datastream::UnitOfMeasurement::toJSONObject(JsonObject& jsonObject) {
  Serial.print("Create inner JSON");
  jsonObject["name"] = this->name_;
  jsonObject["symbol"] = this->symbol;
  jsonObject["definition"] = this->definition;
  
}

void Datastream::setUnitOfMeasurement(String name_, String symbol, String definition) {
  this->unitOfMeasurement.name_ = name_; 
  this->unitOfMeasurement.symbol = symbol; 
  this->unitOfMeasurement.definition = definition; 
}

String Datastream::getSelfId() {
	return selfId;
}

  


