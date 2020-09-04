#include "Datastream_DHT22_humidity.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

DatastreamHumidityDHT22::DatastreamHumidityDHT22(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty): 
	DatastreamHumidityDHT22(thing->getSelfId(), thing->getSerialNumber(), sensor->getSelfId(), sensor->getSerialNumber(), observedProperty->getSelfId()) {
	/*Serial.println("Datastream constructor 1");
	Serial.println("Thing pointer: ");
	Serial.println((int)thing);
	Serial.println(thing->getSerialNumber());*/
	this->thing = thing;
	this->sensor = sensor;
	this->observedProperty = observedProperty;
	//DatastreamHumidityDHT22(thing->getSelfId(), sensor->getSelfId());
  //DatastreamHumidityDHT22("lalala", "abcde");
}

DatastreamHumidityDHT22::DatastreamHumidityDHT22(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId) {
	/*Serial.println("Datastream constructor 2");
	Serial.println(deviceSerial);*/
	this->thingId = thingId;
	this->sensorId = sensorId;
	this->observedPropertyId = observedPropertyId;
  this->name_ = "Relative Humidity Datastream of Crowdsensing Node (DHT22, "+deviceSerial+")";
  this->description = "Datastream for recording relative humidity";
  this->unitOfMeasurement.name_ = "percent";
	this->unitOfMeasurement.symbol = "%";
	this->unitOfMeasurement.definition = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#Percent";
  this->observationType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
	String tempId = "teco.edu:crowdsensor:2019-03:"+deviceSerial+":"+"teco.edu:dht22"+":"+sensorSerial+":hur";
	//Serial.println("tempId: "+ tempId);
	this->selfId = FROSTEntities::Datastream::preambula+":"+toHEXSHA1(tempId).substring(0,7);
	
	
	
}

