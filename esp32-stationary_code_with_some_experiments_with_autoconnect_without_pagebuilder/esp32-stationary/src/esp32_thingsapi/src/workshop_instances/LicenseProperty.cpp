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
  JsonObject licenseObject  = root.createNestedObject("license");
	
  licenseObject["type"] = type;
  licenseObject["owner"] = owner;
  licenseObject["metadata"] = metadata;
}

void LicenseProperty::toJSONString(char* jsonString, size_t length_) {
  Serial.print("License Property: Create JSON String");
  StaticJsonDocument<1024> doc;
  JsonObject root = doc.to<JsonObject>(); //root object filled with further json obejcts
  JsonObject propertyObject  = root.createNestedObject("properties"); 
  toJSONObject(propertyObject);
  
  serializeJson(doc, jsonString, length_);
}