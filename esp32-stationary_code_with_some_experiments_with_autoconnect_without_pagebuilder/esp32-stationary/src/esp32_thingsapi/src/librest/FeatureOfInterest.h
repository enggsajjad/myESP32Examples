#ifndef FEATURE_OF_INTEREST
#define FEATURE_OF_INTEREST

#include <Arduino.h>
#include "ArduinoJson.h"

class FeatureOfInterest
{ 
 private: 
  String name_;
  String description;
  String encodingType;
  String selfId;
  class Feature {
    public:
      String type;
      float coordinates[2];
      void toJSONObject(JsonObject& jsonObject);
  };
  Feature feature;
 public:
  FeatureOfInterest(String name_, String description, String encodingType);
  void setSelfId(String selfId);
  void toJSONString(char* jsonString, size_t length_);
  void setFeature(String type, float* coordinates);
  
}; 

#endif
