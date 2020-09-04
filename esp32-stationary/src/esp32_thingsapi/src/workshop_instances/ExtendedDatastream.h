#ifndef EXTENDED_DATASTREAM_H
#define EXTENDED_DATASTREAM_H

#include "../librest/Datastream.h"

class ExtendedDatastream : public Datastream
{ 
 protected: 
	ToJSONObject* property;
	bool propertySet;
	
  
 public:
  ExtendedDatastream();
  ExtendedDatastream(String name_, String description, String observationType);
  virtual void setProperty(ToJSONObject* property);
  virtual void ToJSONString(char* jsonString, size_t length_); 
  virtual void toJSONObject(JsonObject& root);
}; 

#endif
