#include "LicenseProperty.h"

/*architektor verkackt, entweder es ist ein license property, dann muss es auch ein property key haben oder es ist eine license dann braucht
sie nur den lizenzinhalt, sollte aber auch nur license heißen. Wird jetzt mit toJSONString() gefixed."*/

LicenseProperty::LicenseProperty() {
  
}

LicenseProperty::LicenseProperty(String type, String owner, String metadata) {
  this->type = type;
  this->owner = owner;
  this->metadata = metadata;
}

void LicenseProperty::toJSONObject(JsonObject& root) {
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& licenseObject = jsonBuffer.createObject(); 
	
  licenseObject["type"] = type;
  licenseObject["owner"] = owner;
  licenseObject["metadata"] = metadata;
  root["license"] = licenseObject;
	
}


void LicenseProperty::toJSONString(char* jsonString, size_t length_) {
  Serial.print("License Property: Create JSON String");
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject(); //root object filled with further json obejcts
  JsonObject& propertyObject = jsonBuffer.createObject(); 
  toJSONObject(propertyObject);
  root["properties"] = propertyObject;
	
  root.printTo(jsonString, length_);
}