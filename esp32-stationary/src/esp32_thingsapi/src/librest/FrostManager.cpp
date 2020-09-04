#include "FrostManager.h"
#include "Frost_Server.h"
#include "../workshop_instances/workshop_entities.h"
#include "ArduinoJson.h"
#include <Arduino.h>
#include <HTTPClient.h>


FrostManager::FrostManager() {
  
}
FrostManager::FrostManager(String device_Serial, String SDS011Serial, float* location, String humanReadableLocation) {
	this->device_Serial = device_Serial;
	this->SDS011_Serial = SDS011_Serial;
	this->additionalSensor = AdditionalSensor::NONE;
	this->location[0] = location[0];
	this->location[1] = location[1];
	this->location[2] = location[2]+location[3]; //nach specs von paul zusammenaddieren, feld [3] ist geschätzter wert, feld [2] gemessener wert
	this->humanReadableLocation = humanReadableLocation;
	
}

FrostManager::FrostManager(String device_Serial, String SDS011Serial, AdditionalSensor additionalSensor, String additionalSensorSerial, float* location, String humanReadableLocation) {
	this->device_Serial = device_Serial;
	this->SDS011_Serial = SDS011_Serial;
	this->additionalSensor = additionalSensor;
	this->additionalSensorSerial = additionalSensorSerial;
	this->location[0] = location[0];
	this->location[1] = location[1];
	this->location[2] = location[2]+location[3]; //nach specs von paul zusammenaddieren, feld [3] ist geschätzter wert, feld [2] gemessener wert
	this->humanReadableLocation = humanReadableLocation;
	
}

int FrostManager::createThing() {
	
	return 0;
}

int createBME280() {
	return 0;
}

int createSDS011() {
	return 0;
}


int createObsPropPM10() {
	return 0;
}


int createObsPropPM2_5() {
	return 0;
}

int createObsPropTemperature() {
	return 0;
}


int createObsPropHumidity() {
	return 0;
}


int createObsPropPressure() {
	return 0;
}

void FrostManager::setDatastreamProperty(ToJSONObject* datastreamLicenseProperty) {
	this->datastreamLicenseProperty = datastreamLicenseProperty;
}

int FrostManager::createEntity(String url, ToJSONString* toJSONString) { //it would have also been possible to pass a string reference as second parameter
    Serial.println("HTTP post:");
    HTTPClient http;
    char jsonbuffer[2048];
    size_t j = sizeof(jsonbuffer) / sizeof(jsonbuffer[0]);
    int httpCode;
    Serial.print("URL");
    Serial.print(url);
    Serial.print("JSON_buffer");
    toJSONString->toJSONString(jsonbuffer, j);
    Serial.print(jsonbuffer);
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
		
    httpCode = http.POST(jsonbuffer);
    if(httpCode < 0) {
    Serial.println("Error on HTTP post");
    Serial.println("Code "+String(httpCode));
    }
    else {
    Serial.println("HTTP Code: "+String(httpCode));
    }
    http.end();
    return httpCode;
}

//in this implementation patch cannot be more than 1024 characters in size
int FrostManager::patchEntity(String url, ToJSONString* toJSONString) { 
    Serial.println("HTTP patch:");
    HTTPClient http;
    char jsonbuffer[1024];
    size_t j = sizeof(jsonbuffer) / sizeof(jsonbuffer[0]);
    int httpCode;
    Serial.print("URL");
    Serial.print(url);
    Serial.print("JSON_buffer");
    toJSONString->toJSONString(jsonbuffer, j);
    Serial.print(jsonbuffer);
    http.begin(url);
    http.addHeader("Content-Type", "application/json");
    httpCode = http.PATCH(jsonbuffer);
    if(httpCode < 0) {
    Serial.println("Error on HTTP patch");
    Serial.println("Code "+String(httpCode));
    }
    else {
    Serial.println("HTTP Code: "+String(httpCode));
    }
    http.end();
		return httpCode;
}

String FrostManager::getEntity(String url) { //it would have also been possible to pass a string reference as second parameter
    Serial.println("HTTP get:");
	HTTPClient http;
	int httpCode;
	Serial.println("URL");
	Serial.print(url);

	http.begin(url);
    
    httpCode = http.GET();
    	
    String response = http.getString();
    if(httpCode < 0) {
    Serial.println("Error on HTTP post");
    Serial.println("Code "+String(httpCode));
    }
    else {
    Serial.println("HTTP Code: "+String(httpCode));
    Serial.println("response: " + response);
    }
    http.end();
    return response;
}

//temporary helper function, TODO: change to work for any entity?
int FrostManager::createOrUpdateDatastream(String url, Datastream& entity) { 
    int httpCode;
    String tempURL = FROST_Server::base_url + "/Datastreams('" + entity.getSelfId() + "')"; 
    Serial.println("Attempting to retrieve Datastream from server...");
#ifndef RELEASE
    Serial.println(tempURL);
#endif
    String existingEntity = getEntity(tempURL);
    if (existingEntity == "Nothing found.") { //DS does not exist yet
      Serial.println("Creating Datastream...");
      httpCode = createEntity(FROST_Server::datastreams_url, &entity);
    } else {
	  Serial.println("Patching Datastream...");
      httpCode = patchEntity(tempURL, &entity);
    }
    return httpCode;
}




void FrostManager::postObservation(Observation* observation) {
	createEntity(FROST_Server::observations_url, observation);
}

void FrostManager::createEntities() {
	
	WorkshopLocationEntry myWorkshopLocationEntry(location);
	WorkshopLocation myWorkshopLocation(humanReadableLocation, &myWorkshopLocationEntry);

	CrowdsensingNode myCrowdsensingNode(device_Serial); //Thing
	myCrowdsensingNode.addLocation(&myWorkshopLocation);
    	
    // TEST/HACK/OMG: Patch Entity
    String tempThingURL = FROST_Server::base_url + "/Things('" + myCrowdsensingNode.getSelfId() + "')"; 
    Serial.println("Attempting to retrieve Thing from server...");
    Serial.println(tempThingURL);
    String existingEntity = getEntity(tempThingURL);
    if (existingEntity == "Nothing found.") { //Thing does not exist yet
       createEntity(FROST_Server::things_url, &myCrowdsensingNode);
    } 
    else { // Thing exists
        Serial.print("Attempting to check pending Locations on server...");
		//first check if location exists
		
		//example https://smartaqnet.teco.edu/v1.0/Things('saqn%3At%3Af081359')/Locations?$filter=@iot.id eq 'geo:20.00,10.00,nan'
        String pendingLocationURL = FROST_Server::base_url + "/Locations('" + myWorkshopLocation.getSelfId() + "')";
		
		Serial.print(pendingLocationURL);
        
        String pendingLocationResult = getEntity(pendingLocationURL);
        //String candidateLocation = myWorkshopLocation.ToJSONString();
        if (pendingLocationResult == "Nothing found.") { // Thing exists, but new location does not exist on server
            Serial.println("Pending location not yet on server");
			Serial.println("Create Pending Location...");
            createEntity(FROST_Server::base_url + "/Locations", &myWorkshopLocation);
			Serial.println("Patching Thing...");
			patchEntity(tempThingURL, &myCrowdsensingNode);
        }
        else { // Thing exists and new location also already exists on server
			//check if new location is linked from thing
			Serial.println("Check if thing points to pending location");
			//String thingLocationQueryURL = FROST_Server::base_url + "/Things('" + myCrowdsensingNode.getSelfId() + "')/Locations"+"?$filter=@iot.id eq "+"'"+myWorkshopLocation.getSelfId()+"'";
			//String thingLocationQueryURL="https://smartaqnet-dev.dmz.teco.edu/v1.0/Things('saqn:t:1b26cd7')/Locations?$filter=@iot.id eq 'geo:2.222000,2.222000,nan'"; //wont work
			//String thingLocationQueryURL="https://smartaqnet-dev.dmz.teco.edu/v1.0/Things('saqn:t:1b26cd7')/Locations?$filter=@iot.id%20eq%20%27geo:2.222000,2.222000,nan%27"; //works
			//String thingLocationQueryURL= String("https://smartaqnet-dev.dmz.teco.edu/v1.0/Things('saqn:t:1b26cd7')/Locations")+"?$filter=@iot.id"+"%20"+"eq"+"%20"+"'geo:2.222000,2.222000,nan'"; //works
            String thingLocationQueryURL = FROST_Server::base_url + "/Things('" + myCrowdsensingNode.getSelfId() + "')/Locations"+"?$filter=@iot.id"+"%20"+ "eq"+"%20"+"'"+myWorkshopLocation.getSelfId()+"'";
			//Serial.print(thingLocationQueryURL); //will be also  printed in get Entity request
			String thingLocationQueryResult = getEntity(thingLocationQueryURL);
			Serial.println("thingLocationQueryResult:");
			Serial.println(thingLocationQueryResult);
			//determine size of string if < 50 location is not pendingLocation in thing
			Serial.print("thingLocationQueryResult.length");
			Serial.print(thingLocationQueryResult.length());
			if (thingLocationQueryResult.length()<50) {
				Serial.println("Pending Location not found in Thing...");
				Serial.println("Patching Thing...");
				patchEntity(tempThingURL, &myCrowdsensingNode);
				//Serial.println("done.");
			}
            
            else { // Thing exists and current location in server is already correctly set
                //do nothing
                Serial.println("Data already up-to-date.");
            } 
        }
    }
	

	
	SDS011_Sensor mySDS011Sensor(SDS011_Serial);
	createEntity(FROST_Server::sensors_url, &mySDS011Sensor);

	ObservedProperty_PM2_5 property_pm2_5;
	createEntity(FROST_Server::observedproperties_url, &property_pm2_5);
	
	//if LicenseProperty Pointer is corrupt or NULL ESP will crash
	Datastream_PM2_5 datastream_pm2_5(&myCrowdsensingNode, &mySDS011Sensor, &property_pm2_5);
	datastream_pm2_5.setProperty(datastreamLicenseProperty);
	this->dataStreamPM2_5Id = datastream_pm2_5.getSelfId();
	createOrUpdateDatastream(FROST_Server::datastreams_url, datastream_pm2_5);
	
	ObservedProperty_PM10 property_pm10;
	createEntity(FROST_Server::observedproperties_url, &property_pm10);
	
	Datastream_PM10 datastream_pm10(&myCrowdsensingNode, &mySDS011Sensor, &property_pm10);
	datastream_pm10.setProperty(datastreamLicenseProperty);
	this->dataStreamPM10_Id = datastream_pm10.getSelfId();
	createOrUpdateDatastream(FROST_Server::datastreams_url, datastream_pm10);

	ObservedProperty_Humidity property_Humidity;
	createEntity(FROST_Server::observedproperties_url, &property_Humidity);
	
	ObservedProperty_Temperature property_Temperature;
	//createEntity(FROST_Server::observedproperties_url, &property_Temperature);
	createEntity(FROST_Server::observedproperties_url, &property_Temperature);
	
	//Distinguish between DHT22, BME280 or no Sensors
	if(additionalSensor == BME280) {
		BME280_Sensor mybme280Sensor(additionalSensorSerial);
		createEntity(FROST_Server::sensors_url, &mybme280Sensor);
	
		ObservedProperty_Pressure property_Pressure;
		createEntity(FROST_Server::observedproperties_url, &property_Pressure);
		
		DatastreamHumidityBME280 datastreamHumidityBME280(&myCrowdsensingNode, &mybme280Sensor, &property_Humidity);
		datastreamHumidityBME280.setProperty(datastreamLicenseProperty);
		this->dataStreamHumidityBME280_Id = datastreamHumidityBME280.getSelfId();
		createOrUpdateDatastream(FROST_Server::datastreams_url, datastreamHumidityBME280);

		DatastreamPressureBME280 datastreamPressureBME280(&myCrowdsensingNode, &mybme280Sensor, &property_Pressure);
		datastreamPressureBME280.setProperty(datastreamLicenseProperty);
		this->dataStreamPressureBME280_Id = datastreamPressureBME280.getSelfId();
		createOrUpdateDatastream(FROST_Server::datastreams_url, datastreamPressureBME280);

		DatastreamTemperatureBME280 datastreamTemperatureBME280(&myCrowdsensingNode, &mybme280Sensor, &property_Temperature);
		datastreamTemperatureBME280.setProperty(datastreamLicenseProperty);
		this->dataStreamTemperatureBME280_Id = datastreamTemperatureBME280.getSelfId();
		createOrUpdateDatastream(FROST_Server::datastreams_url, datastreamTemperatureBME280);
	}
	else if(additionalSensor == DHT22) {
		
		DHT22_Sensor myDHT22Sensor(additionalSensorSerial);
		createEntity(FROST_Server::sensors_url, &myDHT22Sensor);
	
		DatastreamHumidityDHT22 datastreamHumidityDHT22(&myCrowdsensingNode, &myDHT22Sensor, &property_Humidity);
		this->dataStreamHumidityDHT22_Id = datastreamHumidityDHT22.getSelfId();
		createOrUpdateDatastream(FROST_Server::datastreams_url, datastreamHumidityDHT22);


		DatastreamTemperatureDHT22 datastreamTemperatureDHT22(&myCrowdsensingNode, &myDHT22Sensor, &property_Temperature);
		this->dataStreamTemperatureDHT22_Id = datastreamTemperatureDHT22.getSelfId();
		createOrUpdateDatastream(FROST_Server::datastreams_url, datastreamTemperatureDHT22);
		
	}
	else if(additionalSensor == NONE) {
	}
	
	
}


