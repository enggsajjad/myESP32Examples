#include "ObservedProperty_PM10.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ObservedProperty_PM10::ObservedProperty_PM10() {
  this->name_ = "PM10";
  this->description = "Mass concentration of Particulate Matter with a diameter of equal or less than 10 micrometers in air.";
  this->definition = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#mass_concentration_of_pm10_ambient_aerosol_particles_in_air";
	this->selfId = FROSTEntities::ObservedProperty::preambula+":"+"mcpm10";
}
