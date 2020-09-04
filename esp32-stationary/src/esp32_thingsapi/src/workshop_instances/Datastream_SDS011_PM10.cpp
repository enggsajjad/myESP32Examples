#include "Datastream_SDS011_PM10.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

Datastream_PM10::Datastream_PM10(Thing* thing, Sensor* sensor, ObservedProperty* observedProperty): 
	Datastream_PM10(thing->getSelfId(), thing->getSerialNumber(), sensor->getSelfId(), sensor->getSerialNumber(), observedProperty->getSelfId()) {
	/*Serial.println("Datastream constructor 1");
	Serial.println("Thing pointer: ");
	Serial.println((int)thing);
	Serial.println(thing->getSerialNumber());*/
	this->thing = thing;
	this->sensor = sensor;
	this->observedProperty = observedProperty;
	//Datastream_PM10(thing->getSelfId(), sensor->getSelfId());
  //Datastream_PM10("lalala", "abcde");
}

Datastream_PM10::Datastream_PM10(String thingId, String deviceSerial, String sensorId, String sensorSerial, String observedPropertyId) {
	/*Serial.println("Datastream constructor 2");
	Serial.println(deviceSerial);*/
	this->thingId = thingId;
	this->sensorId = sensorId;
	this->observedPropertyId = observedPropertyId;
  this->name_ = "PM10 Datastream of Crowdsensing Node (SDS011, "+deviceSerial+")";
  this->description = "Datastream for recording Particulate Matter";
  this->unitOfMeasurement.name_ = "microgram per cubic meter";
	this->unitOfMeasurement.symbol = "ug/m^3";
	this->unitOfMeasurement.definition = "http://www.qudt.org/qudt/owl/1.0.0/unit/Instances.html#KilogramPerCubicMeter";
  this->observationType = "http://www.opengis.net/def/observationType/OGC-OM/2.0/OM_Measurement";
	String tempId = "teco.edu:crowdsensor:2019-03:"+deviceSerial+":"+"teco.edu:sds011"+":"+sensorSerial+":pm10";
	//Serial.println("tempId: "+ tempId);
	this->selfId = FROSTEntities::Datastream::preambula+":"+toHEXSHA1(tempId).substring(0,7);
}

