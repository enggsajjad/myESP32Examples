#include "Datastream_BME280_pressure.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

DatastreamPressureBME280::DatastreamPressureBME280(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty): 
	DatastreamPressureBME280(thing->getSelfId(), thing->getSerialNumber(), sensor->getSelfId(), sensor->getSerialNumber(), observedProperty->getSelfId()) {
	Serial.println("Datastream constructor 1");
	Serial.println("Thing pointer: ");
	Serial.println((int)thing);
	Serial.println(thing->getSerialNumber());
	this->thing = thing;
	this->sensor = sensor;
	this->observedProperty = observedProperty;
	//DatastreamPressureBME280(thing->getSelfId(), sensor->getSelfId());
  //DatastreamPressureBME280("lalala", "abcde");
}

DatastreamPressureBME280::DatastreamPressureBME280(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId) {
	Serial.println("Datastream constructor 2");
	Serial.println(deviceSerial);
	this->thingId = thingId;
	this->sensorId = sensorId;
	this->observedPropertyId = observedPropertyId;
  this->name_ = "Atmospheric Pressure Datastream of Crowdsensing Node (BME280, "+deviceSerial+")";
  this->description = "Datastream for recording pressure";
  this->unitOfMeasurement.name_ = "Hectopascal";
	this->unitOfMeasurement.symbol = "hPa";
	this->unitOfMeasurement.definition = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#Pascal";
  this->observationType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
	String tempId = "teco.edu:crowdsensor:2019-03:"+deviceSerial+":"+"teco.edu:bme280"+":"+sensorSerial+":plev";
	//Serial.println("tempId: "+ tempId);
	this->selfId = FROSTEntities::Datastream::preambula+":"+toHEXSHA1(tempId).substring(0,7);
	
	
	
}

