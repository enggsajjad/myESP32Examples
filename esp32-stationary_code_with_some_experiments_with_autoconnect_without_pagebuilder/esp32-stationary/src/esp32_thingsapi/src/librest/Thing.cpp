#include "Thing.h"
#include "ArduinoJson.h"
#include <Arduino.h>

Thing::Thing() {
  
}
Thing::Thing(String name_, String description, String serialNumber) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
}

Thing::Thing(String name_, String description, String serialNumber, String selfId) {
  this->name_ = name_;
  this->description = description;
  this->serialNumber = serialNumber;
  this->selfId = selfId;
}



void Thing::setLocationId(String locationId) {
  this->locationId = locationId;
  locationIdSet = true;
  
}

void Thing::toJSONObject(JsonObject& root) {
	//alle bezeichner zentral verwalten in einem header
	root["name"] = name_; //implicit allocation as object
	root["description"] = description;
	root["@iot.id"] = selfId;

	if(locationIdSet) {
		//not implemented in json6 yet
		/*JsonObject locationObject = doc.createNestedObject();
		locationObject["@iot.id"] = locationId;

		JsonArray* locationArray = NULL;
		locationArray = &doc.createNestedArray();
		locationArray->add(locationObject);

		root["Locations"] = *locationArray;*/
	}
}
	

	
//location not yet implemented
void Thing::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Thing: Create JSON String");
  StaticJsonDocument<2048> doc;
  JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
  toJSONObject(root);
  serializeJson(doc, jsonString, length_);
	
}

String Thing::getSelfId() {
	return selfId;
}

String Thing::getSerialNumber() {
	return serialNumber;
}
  


