#include "ObservedProperty_temperature.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ObservedProperty_Temperature::ObservedProperty_Temperature() {
  this->name_ = "Air Temperature";
  this->description = "Air temperature is the bulk temperature of the air, not the surface (skin) temperature.";
  this->definition = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#air_temperature";
	this->selfId = FROSTEntities::ObservedProperty::preambula+":"+"ta";

}
