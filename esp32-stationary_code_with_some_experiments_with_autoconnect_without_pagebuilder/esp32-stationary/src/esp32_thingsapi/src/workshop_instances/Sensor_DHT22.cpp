#include "Sensor_DHT22.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

DHT22_Sensor::DHT22_Sensor(String serialNumber) {
  this->name_ = "DHT22";
  this->description = "A sensor measuring temperature and humidity";
  this->encodingType = "application/pdf";
  this->metadata = "https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf";
	String tempId = "teco.edu:dht22";
	this->selfId = FROSTEntities::Sensor::preambula+":"+toHEXSHA1(tempId).substring(0,7);
	this->serialNumber = serialNumber;
}
