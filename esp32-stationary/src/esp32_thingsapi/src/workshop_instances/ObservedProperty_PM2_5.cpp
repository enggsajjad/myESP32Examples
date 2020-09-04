#include "ObservedProperty_PM2_5.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

ObservedProperty_PM2_5::ObservedProperty_PM2_5() {
  this->name_ = "PM2.5";
  this->description = "Mass concentration of Particulate Matter with a diameter of equal or less than 2.5 micrometers in air.";
  this->definition = "http://cfconventions.org/Data/cf-standard-names/63/build/cf-standard-name-table.html#mass_concentration_of_pm2p5_ambient_aerosol_particles_in_air";
	this->selfId = FROSTEntities::ObservedProperty::preambula+":"+"mcpm2p5";
}
