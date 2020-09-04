#include "Observation.h"
#include "ArduinoJson.h"
#include <Arduino.h>



Observation::Observation() {
  
}

Observation::Observation(String datastreamId, String phenomenonTime, String resultTime, double result) {
  this->phenomenonTime = phenomenonTime;
  this->result = result;
  this->resultTime = resultTime;
	this->datastreamId = datastreamId;
}

/*void Observation::setSelfId(String selfId) {
  this->selfId = selfId;
}*/



void Observation::toJSONString(char* jsonString, size_t length_) {
  Serial.print("Create JSON");
  StaticJsonBuffer<256> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); //root object filled with further json obejcts

  root["phenomenonTime"] = phenomenonTime;
  root["resultTime"] = resultTime;
  root["result"] = result;
	
	JsonObject& dataStreamObject = jsonBuffer.createObject();
  dataStreamObject["@iot.id"] = datastreamId;
	
	root["Datastream"] = dataStreamObject;
  
  root.printTo(jsonString, length_);

  
}
