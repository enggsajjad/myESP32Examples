#pragma once
#include <WiFi.h>
#include <HTTPClient.h>
#include "wlan_credentials.h"
#include "ArduinoJson.h"


#include "src/librest/rest_entities.h"
#include "src/workshop_instances/workshop_entities.h"
#include "src/workshop_instances/EntityNaming.h"
#include "src/librest/Frost_Server.h"
#include "src/librest/FrostManager.h"
#include "src/librest/FrostUtilities.h"
#include "src/workshop_instances/FirmwareProperty.h"
#include "src/workshop_instances/LicenseProperty.h"


boolean factoryfresh = false; //if the node hasn't been used before
const int WLAN_TIMEOUT_MS = 30000;
byte esp32_MAC[6];
double temperature = 0.000111; // nach double konvertieren
double humidity = 0.000222;
double pressure = 0.000333;
double pm10 = 0.000444;
double pm2_5 = 0.000555;

enum States {
  WLANX_CONNECT,
  WLANX_ERROR,
  THINGS_SERVER_PROBING,
  THINGS_SERVER_ERROR,
  HTTP_REQUEST_ERROR,
  SENSORS_READING,
  THINGS_CREATE_STRUCTURE,
  IDLE_,
  POSTING_OBSERVATIONS
};

States program_state = WLANX_CONNECT;
HTTPClient http;

//const char* FROST_SERVER_URL = "http://smartaqnet-dev.teco.edu:8080/FROST-Server/v1.0";
const char* FROST_SERVER_URL = "http://192.168.1.34:8080/FROST-Server/v1.0";
const char* OPERATOR_URL = "tec.edu";

String MAC = "F0:0F:00:00";

float coordinates[] = {8.24, 49.0, 1.0, 2.2};

//FirmwareProperty
FirmwareProperty myFirmwareProperty("10.07.2019", "intermediate jan 0.5");


//LicenseProperty
LicenseProperty myLicenseProperty("GNU LICENSE", "Max Musterfrau", "https://www.gnu.org/licenses/gpl-3.0.en.html");


WorkshopLocationEntry myWorkshopLocationEntry(coordinates);
WorkshopLocation myWorkshopLocation("86150 Augsburg", &myWorkshopLocationEntry);

CrowdsensingNode myCrowdsensingNode(MAC); 

/*BME280_Sensor mybme280Sensor("no serial");
SDS011_Sensor mySDS011Sensor("no serial");

ObservedProperty_PM2_5 property_pm2_5;
Datastream_PM2_5 datastream_pm2_5(&myCrowdsensingNode, &mySDS011Sensor, &property_pm2_5);

ObservedProperty_PM10 property_pm10;
Datastream_PM10 datastream_pm10(&myCrowdsensingNode, &mySDS011Sensor, &property_pm10);

ObservedProperty_Humidity property_Humidity;
Datastream_Humidity Datastream_Humidity(&myCrowdsensingNode, &mybme280Sensor, &property_Humidity);

ObservedProperty_Pressure property_Pressure;
Datastream_Pressure Datastream_Pressure(&myCrowdsensingNode, &mybme280Sensor, &property_Pressure);

ObservedProperty_Temperature property_Temperature;
Datastream_Temperature Datastream_Temperature(&myCrowdsensingNode, &mybme280Sensor, &property_Temperature);*/

// frostManager(mac, sds011_SN, bme280_SN, location, humanreadible_loc)

// location: [float long, float lat, float alt]
// MAC-Adresse: die esp32id (letzte 3 stellen von mac und nach dezimal konvertiert)
// wenn keine SN, dann leerer String ""
//FrostManager frostManager(MAC, "no", FrostManager::AdditionalSensor::BME280, "", coordinates, "Lilienweg 7b");

//frostmanager as of 10.07.2019
FrostManager frostManager(MAC, "no", FrostManager::AdditionalSensor::BME280, "", coordinates, "Lilienweg 7b");

//FrostManager::UsedSensor myenum = FrostManager::BME280;

// ab hier egal

void printMAC() {
  Serial.print("MAC: ");
  Serial.print(esp32_MAC[5], HEX);
  Serial.print(":");
  Serial.print(esp32_MAC[4], HEX);
  Serial.print(":");
  Serial.print(esp32_MAC[3], HEX);
  Serial.print(":");
  Serial.print(esp32_MAC[2], HEX);
  Serial.print(":");
  Serial.print(esp32_MAC[1], HEX);
  Serial.print(":");
  Serial.println(esp32_MAC[0], HEX);
}

void connectToWLAN() {
  long currentTime = millis();
  long maxAllowedTime = currentTime + WLAN_TIMEOUT_MS;
  Serial.println("Establishing connection to WiFi..");
  WiFi.begin(ssid, password);

  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      if (millis() > maxAllowedTime) {
        program_state = WLANX_ERROR;
        break;
      }
    }
    else {
      Serial.println("Connected to network");
      WiFi.macAddress(esp32_MAC);
      printMAC();
      program_state = THINGS_SERVER_PROBING;
      break;
    }
  }
}





void setup() {

  Serial.println("Setup ...");
  Serial.begin(115200);
  //char* charbuffer[2048];
   char charbuffer[2048];
  size_t n = sizeof(charbuffer) / sizeof(charbuffer[0]);

  myFirmwareProperty.toJSONString(charbuffer, n);
  Serial.println(charbuffer);

  myLicenseProperty.toJSONString(charbuffer, n);
  Serial.println(charbuffer);

  

  myCrowdsensingNode.addLocation(&myWorkshopLocation);
  Serial.println("Constants: ");
  Serial.println(FROST_Server::base_url);
  Serial.println(FROST_Server::things_url);
  Serial.println(FROST_Server::datastreams_url);

  frostManager.setDatastreamProperty(&myLicenseProperty);
  frostManager.setThingProperty(&myFirmwareProperty);
  
  Serial.println("Setup completed!");



}

void loop() {
  

  if (program_state == WLANX_CONNECT) {
    connectToWLAN();
  }
  if (program_state == WLANX_ERROR) {
    Serial.println("WLAN Error");
  }
  if (program_state == THINGS_SERVER_PROBING) {
    http.begin("https://smartaqnet-dev.dmz.teco.edu/v1.0/");
    int httpCode = http.GET();
    if (httpCode < 0) {
      program_state = HTTP_REQUEST_ERROR;
      Serial.println("Error on HTTP request");
    }
    else if (httpCode != 200) {
      program_state = THINGS_SERVER_ERROR;
      Serial.println("Error on HTTP request");
    }
    else {
      Serial.println("Things Server Ready");
      program_state = THINGS_CREATE_STRUCTURE; //check if the structure has already been created, entweder im EEPROM oder per server request
    }
  }
  if (program_state == IDLE_) {
    //arm sensors
  }

  // hier kommen die eigentlich daten fÃ¼r den POST rein
      // die dataStreamTemperature_Id kommt vom frost manager direkt
      // erste Zeit: wenn messung gestartet (also vor sdsread()
      // zweite Zeit: wenn messung fertig, also wenn werte Ã¼bergeben werden (also wenn sdsread() fertig)
      // parameter kann nicht weg gelassen werden, also nur senden, wenn sensor angeschlossen
  if (program_state == THINGS_CREATE_STRUCTURE) {
    Serial.println("Create things structure");
    // muss aufgerufen werden, bevor etwas gesendet werden kann. dauert mehrere minuten (also im setup). 
    frostManager.createEntities();
    program_state = POSTING_OBSERVATIONS;
  }
  if (program_state == POSTING_OBSERVATIONS) {
    Serial.println("Posting Observations");
    if(frostManager.additionalSensor == FrostManager::AdditionalSensor::BME280) {
      { 
        Observation observation(frostManager.dataStreamTemperatureBME280_Id, "2017-02-07T18:02:00.000Z", "2017-02-07T18:02:00.000Z", temperature);
        //temperature += 0.0001;
        frostManager.postObservation(&observation);
      }
  
      {
        Observation observation(frostManager.dataStreamHumidityBME280_Id, "2017-02-07T18:02:00.000Z", "2017-02-07T18:02:00.000Z", humidity);
        //humidity -= 0.0001;
        frostManager.postObservation(&observation);
      }
      {
        Observation observation(frostManager.dataStreamPressureBME280_Id, "2017-02-07T18:02:00.000Z", "2017-02-07T18:02:00.000Z", pressure);
        //humidity += 0.00012;
        frostManager.postObservation(&observation);
      }
    }
    else if(frostManager.additionalSensor == FrostManager::AdditionalSensor::DHT22) {
      { 
        Observation observation(frostManager.dataStreamTemperatureDHT22_Id, "2017-02-07T18:02:00.000Z", "2017-02-07T18:02:00.000Z", temperature);
        //temperature += 0.0001;
        frostManager.postObservation(&observation);
      }
  
      {
        Observation observation(frostManager.dataStreamHumidityDHT22_Id, "2017-02-07T18:02:00.000Z", "2017-02-07T18:02:00.000Z", humidity);
        //humidity -= 0.0001;
        frostManager.postObservation(&observation);
      }
    }
    {
      Observation observation(frostManager.dataStreamPM10_Id, "2017-02-07T18:02:00.000Z", "2017-02-07T18:02:00.000Z", pm10);
      //pm10 += 0.0019;
      frostManager.postObservation(&observation);
    }
    {
      Observation observation(frostManager.dataStreamPM2_5Id, "2017-02-07T18:02:00.000Z", "2017-02-07T18:02:00.000Z", pm2_5);
      //pm2_5 += 0.00017;
      frostManager.postObservation(&observation);
    }
    
  }

  delay(10000);

}
