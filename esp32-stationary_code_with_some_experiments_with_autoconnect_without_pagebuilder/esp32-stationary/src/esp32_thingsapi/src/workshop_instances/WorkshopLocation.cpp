#include "WorkshopLocation.h"
#include "EntityNaming.h"
#include "../librest/FrostUtilities.h"
#include "ArduinoJson.h"
#include <Arduino.h>

WorkshopLocation::WorkshopLocation(String humanReadableLocation, LocationEntry* locationEntry) {
	
  this->locationEntry = locationEntry;
	this->name_ = humanReadableLocation;
	String tempId = this->locationEntry->getIDRepresentation();
  this->selfId = FROSTEntities::Location::preambula+":"+tempId;
  this->encodingType = "application/vnd.geo+json";
}



