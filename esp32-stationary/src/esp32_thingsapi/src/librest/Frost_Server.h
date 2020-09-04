#ifndef FROST_SERVER_H
#define FROST_SERVER_H

#include <Arduino.h>

//#define BASE_URL "http://192.168.1.34:8080/FROST-Server/v1.0/"
//#define CONCAT(BASE_URL, dir) BASE_URL "/" dir

namespace FROST_Server
{ 
	extern String base_url;
	extern String datastreams_url;
	extern String multidatastreams_url;
	extern String featureofinterest_url;
	extern String historicallocations_url;
	extern String locations_url;
	extern String observations_url;
	extern String observedproperties_url;
	extern String sensors_url;
	extern String things_url;
  
}

#endif
