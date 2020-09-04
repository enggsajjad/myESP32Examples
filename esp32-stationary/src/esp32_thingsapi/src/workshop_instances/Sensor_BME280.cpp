#include "Sensor_BME280.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

BME280_Sensor::BME280_Sensor(String serialNumber) {
  this->name_ = "Bosch BME280";
  this->description = "A sensor measuring temperature, atmospheric pressure and humidity";
  this->encodingType = "application/pdf";
  this->metadata = "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf";
	String tempId = "teco.edu:bme280";
	this->selfId = FROSTEntities::Sensor::preambula+":"+toHEXSHA1(tempId).substring(0,7);
	this->serialNumber = serialNumber;
}
