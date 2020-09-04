#ifndef OBSERVATION
#define OBSERVATION

#include "ToJSONString.h"
#include "Datastream.h"
#include <Arduino.h>


class Observation : public ToJSONString
{ 
 private: 
  String phenomenonTime;
	String resultTime;
  double result; //subclass and overwrite if other result type is neccessary
  //String selfId;
	//String featureOfInterestId;
  String datastreamId;
	Datastream* datastream;
  
 public:
	Observation();
  Observation(String datastreamId, String phenomenonTime, String resultTime, double result);
  //void setSelfId(String selfId);
  //void setFeatureOfInterestId(String featureOfInterestId);
  void setDatastreamId(String datastreamId);
  void toJSONString(char* jsonString, size_t length_);
  
}; 

#endif
