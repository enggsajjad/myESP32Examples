#include "Frost_Server.h"


#ifdef RELEASE
  #define BASE_URL "http://api.smartaq.net/v1.0"
#else
 #define BASE_URL "https://smartaqnet-dev.dmz.teco.edu/v1.0"
#endif 

#define CONCAT(BASE_URL, dir) BASE_URL "/" dir

namespace FROST_Server
{ 
	String base_url = BASE_URL;
	String datastreams_url = CONCAT(BASE_URL, "Datastreams");
	String multidatastreams_url = CONCAT(BASE_URL, "MultiDatastreams");
	String featureofinterest_url = CONCAT(BASE_URL, "FeaturesOfInterest");
	String historicallocations_url = CONCAT(BASE_URL, "HistoricalLocations");
	String locations_url = CONCAT(BASE_URL, "Locations");
	String observations_url = CONCAT(BASE_URL, "Observations");
	String observedproperties_url = CONCAT(BASE_URL, "ObservedProperties");
	String sensors_url = CONCAT(BASE_URL, "Sensors");
	String things_url = CONCAT(BASE_URL, "Things");
  
}

