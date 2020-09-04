#include "FirmwareProperty.h"

/*architektor verkackt, toJSONString muss dieses Objekt eigentlich genau abbilden. Bequemlichkeitshalber wurde aber hier das äußere Properties-Objekt definiert. Muss eigentlich 
in der Aufrufenden klasse, die ein Properties-Objekt enthält erzeugt werden. Beim Patchen gibt es aber keine äußere aufrufende Klasse*/

FirmwareProperty::FirmwareProperty() {
  
}

FirmwareProperty::FirmwareProperty(String date, String version) {
  this->date = date;
  this->version = version;
}

void FirmwareProperty::toJSONObject(JsonObject& root) {
  JsonObject firmwareObject  = root.createNestedObject("FW_VERSION");
  //slightly different structore here than usual, cause date (key) itsels is varying
  firmwareObject[date] = version;	
}


void FirmwareProperty::toJSONString(char* jsonString, size_t length_) {
  Serial.print("License Property: Create JSON String");
  StaticJsonDocument<1024> doc;
  JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
  JsonObject propertyObject  = root.createNestedObject("properties");
	
  toJSONObject(propertyObject);
  
  serializeJson(doc, jsonString, length_);
}