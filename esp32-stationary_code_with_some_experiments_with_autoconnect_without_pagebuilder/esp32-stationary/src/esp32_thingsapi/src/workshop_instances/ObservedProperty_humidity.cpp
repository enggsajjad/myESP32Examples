#include "ObservedProperty_humidity.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ObservedProperty_Humidity::ObservedProperty_Humidity() {
  this->name_ = "Relative Humidity";
  this->description = "Relative humidity is the ratio of the partial pressure of water vapor to the equilibrium vapor pressure of water at a given temperature.";
  this->definition = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#relative_humidity";
	this->selfId = FROSTEntities::ObservedProperty::preambula+":"+"hur";
}
