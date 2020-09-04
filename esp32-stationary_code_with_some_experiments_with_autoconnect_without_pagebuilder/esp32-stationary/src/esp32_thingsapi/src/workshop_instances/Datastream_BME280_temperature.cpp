#include "Datastream_BME280_temperature.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

DatastreamTemperatureBME280::DatastreamTemperatureBME280(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty): 
	DatastreamTemperatureBME280(thing->getSelfId(), thing->getSerialNumber(), sensor->getSelfId(), sensor->getSerialNumber(), observedProperty->getSelfId()) {
	/*Serial.println("Datastream constructor 1");
	Serial.println("Thing pointer: ");
	Serial.println((int)thing);
	Serial.println(thing->getSerialNumber());*/
	this->thing = thing;
	this->sensor = sensor;
	this->observedProperty = observedProperty;
	//DatastreamTemperatureBME280(thing->getSelfId(), sensor->getSelfId());
  //DatastreamTemperatureBME280("lalala", "abcde");
}

DatastreamTemperatureBME280::DatastreamTemperatureBME280(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId) {
	Serial.println("Datastream constructor 2");
	Serial.println(deviceSerial);
	this->thingId = thingId;
	this->sensorId = sensorId;
	this->observedPropertyId = observedPropertyId;
  this->name_ = "Air Temperature Datastream of Crowdsensing Node (BME280, "+deviceSerial+")";
  this->description = "Datastream for recording temperature";
  this->unitOfMeasurement.name_ = "Degree Celsius";
	this->unitOfMeasurement.symbol = "degC";
	this->unitOfMeasurement.definition = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#DegreeCelsius";
  this->observationType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
	String tempId = "teco.edu:crowdsensor:2019-03:"+deviceSerial+":"+"teco.edu:bme280"+":"+sensorSerial+":ta";
	//Serial.println("tempId: "+ tempId);
	this->selfId = FROSTEntities::Datastream::preambula+":"+toHEXSHA1(tempId).substring(0,7);
	
	
	
}

