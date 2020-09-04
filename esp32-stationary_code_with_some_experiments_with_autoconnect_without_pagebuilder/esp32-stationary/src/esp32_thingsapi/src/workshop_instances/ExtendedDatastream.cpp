
#include "ExtendedDatastream.h"

ExtendedDatastream::ExtendedDatastream() {
 
}

ExtendedDatastream::ExtendedDatastream(String name_, String description, String observationType): 
	Datastream(name_, description, observationType) {
	this->propertySet = false;
}



//sets, resepectiveley resets property 
void ExtendedDatastream::setProperty(ToJSONObject* property){
	this->property = property;
	this->propertySet = true;
}

void ExtendedDatastream::toJSONObject(JsonObject& root){
	Serial.print("ExtendetDatastream:toJSONObject");
	if(propertySet) {
		Serial.println("Datastream property set");
		JsonObject propertyObject = root.createNestedObject("properties");
		property->toJSONObject(propertyObject); //generates key-value pairs
	}
	else {
		Serial.println("No Datastream property set");
	}
	Datastream::toJSONObject(root);
	
	
	
	
}

void ExtendedDatastream::ToJSONString(char* jsonString, size_t length_){
	Serial.print("ExtendetDatastream: Create JSON String");
	StaticJsonDocument<2048> doc;
	JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
	toJSONObject(root);
 	serializeJson(doc, jsonString, length_);
}

