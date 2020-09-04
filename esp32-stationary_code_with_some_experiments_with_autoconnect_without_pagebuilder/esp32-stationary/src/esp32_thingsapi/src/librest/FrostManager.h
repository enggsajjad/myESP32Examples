#ifndef FROST_MANAGER_H
#define FROST_MANAGER_H

#include "Datastream.h"
#include "Observation.h"
#include "ToJSONString.h"
#include <Arduino.h>
#include "../workshop_instances/LicenseProperty.h"


class FrostManager
{ 
	public:
		enum AdditionalSensor {
				NONE = 0,
				BME280 = 1,
				DHT22 = 2,
		};
	
	private: 
		String device_Serial;
		String SDS011_Serial;
		String BME280_Serial;
		float location[3];
		String humanReadableLocation;
		String additionalSensorSerial;
		ToJSONObject* datastreamLicenseProperty;
		ToJSONObject* thingProperty;
		
	public:
		String dataStreamPM10_Id;
		String dataStreamPM2_5Id;
		String dataStreamTemperatureBME280_Id;
		String dataStreamHumidityBME280_Id;
		String dataStreamPressureBME280_Id;
		String dataStreamTemperatureDHT22_Id;
		String dataStreamHumidityDHT22_Id;
		AdditionalSensor additionalSensor;
		
	
  
	public:
		FrostManager();
		FrostManager(String device_Serial, String SDS011Serial, float* location, String humanReadableLocation);
		FrostManager(String device_Serial, String SDS011Serial, AdditionalSensor additionalSensor, String additionalSensorSerial, float* location, String humanReadableLocation);
		void createEntities();
		void postObservation(Observation* observation);
		int createEntity(String url, ToJSONString* toJSONString);
        String getEntity(String url);
		int patchEntity(String url, ToJSONString* toJSONString);
		void setDatastreamProperty(ToJSONObject* datastreamLicenseProperty);
		void setThingProperty(ToJSONObject* thingProperty);
	
	private:
		int createThing();
		int createBME280();
		int createSDS011();
		int createObsPropPM10();
		int createObsPropPM2_5();
		int createObsPropTemperature();
		int createObsPropHumidity();
		int createObsPropPressure();
        int createOrUpdateDatastream(String url, Datastream& entity);

		
  
  
}; 

#endif
