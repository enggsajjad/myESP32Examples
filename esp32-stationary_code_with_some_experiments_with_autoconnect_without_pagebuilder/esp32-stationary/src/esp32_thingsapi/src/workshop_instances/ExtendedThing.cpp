#include "ExtendedThing.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ExtendedThing::ExtendedThing() {
  
}
ExtendedThing::ExtendedThing(String name_, String description, String serialNumber): 
	Thing(name_, description, serialNumber) {
	this->propertySet = false;
}

ExtendedThing::ExtendedThing(String name_, String description, String serialNumber, String selfId): 
  Thing(name_, description, serialNumber, selfId) {
	this->propertySet = false;
}

//sets, resepectiveley resets property 
void ExtendedThing::setProperty(ToJSONObject* property){
	this->property = property;
	this->propertySet = true;
}

//location not yet implemented
void ExtendedThing::toJSONObject(JsonObject& root) {
  Serial.print("ExtendedThing:toJSONObject");
	if(propertySet) {
		Serial.println("ExtendedThing property set");
		JsonObject propertyObject = root.createNestedObject("properties");
		property->toJSONObject(propertyObject); //generates key-value pairs
		
	}
	else {
		Serial.println("No ExtendedThing property set");
	}
	Thing::toJSONObject(root);
}

//location not yet implemented
void ExtendedThing::toJSONString(char* jsonString, size_t length_) {
  Serial.print("ExtendedThing: Create JSON String");
	StaticJsonDocument<2048> doc;
	JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
  	toJSONObject(root);
  	serializeJson(doc, jsonString, length_);
}




