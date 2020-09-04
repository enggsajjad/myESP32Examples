#include "ObservedProperty_pressure.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ObservedProperty_Pressure::ObservedProperty_Pressure() {
  this->name_ = "Air Pressure";
  this->description = "Air pressure is the force per unit area which would be exerted when the moving gas molecules of which the air is composed strike a theoretical surface of any orientation.";
  this->definition = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#air_pressure";
	this->selfId = FROSTEntities::ObservedProperty::preambula+":"+"plev";
}
