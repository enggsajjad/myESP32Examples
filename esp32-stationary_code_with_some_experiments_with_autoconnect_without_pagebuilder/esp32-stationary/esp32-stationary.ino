/**
 * \file esp32-stationary.ino
 *
 * \brief This is the Arduino .ino file.
 *
 * This the Arduino INO implementation file, which implement the overall flow of the project by calling different files/functions.
 * 
 * \date 19.08.2019
 */
#include "main.h"

// vars to store sensor results
/// sensor results for dust sensor pm10
float pm10;
/// sensor results for dust sensor pm25
float pm25;
/// temperature values
float temp;
/// humidity values
float hum;
/// pressure values
float atm;
/// time of measurement for sds reading
String timeofmeas_sds;
/// time of measurement for rht reading
String timeofmeas_rht;
/// sds status
int status_sds;


/**************************************************************************/
/*!
    @brief  initialization of peripherals attached to ESP32 board
    @returns void
*/
/**************************************************************************/
void setup() 
{
  pinMode(RST, OUTPUT);
  digitalWrite(RST,HIGH);


  // make serial monitor printing available
  Serial.begin(115200);

  digitalWrite(RST,LOW);
  delay(100);
  digitalWrite(RST,HIGH);
  delay(100);

  // print FW version and devmode|release setting over serial
  Serial.print("\nVERSION: Crowdsensor FW ");
  Serial.println(FIRMWARE_VERSION);
  #ifndef RELEASE
  Serial.println("\nWARNING: YOU ARE RUNNING A DEVELOPMENT VERSION OF THIS FIRMWARE!!!\n");
  #endif


  float latitude = get_value("user_data", "latitude").toFloat();
  float longitude = get_value("user_data", "longitude").toFloat();
  float abovenn = 0;
  // if aboveground is NAN, then abovenn and aboveground will not be sent to frost server (location then only consists of lat and long
  // format for coordinates: {longitude, latitude, abovenn, NAN}
  float coordinates[] = {longitude, latitude, abovenn, NAN}; 
  frostManager = new FrostManager(getChipId(), String(get_value("user_data", "sds011id")), FrostManager::AdditionalSensor::BME280 , "", coordinates, String(get_value("user_data", "address")));

  // initialse data license
  #ifndef RELEASE
  Serial.println("Building license object");
  #endif
  String license_type = get_value("user_data", "data_license");
  if (!(license_type == "")) 
  {
    String license_owner = get_value("user_data", "data_owner");
    String license_url = get_value("user_data", "data_url");
    mylicenseproperty = new LicenseProperty(license_type,license_owner,license_url);
    #ifndef RELEASE
    Serial.println(license_type + " " + license_owner + " " + license_url);
    #endif
  }
  // setup display
  disp.init();
  disp.setFont(Syncopate_Bold_9);
  
  // print FW version on display for 2 seconds
  disp.drawString(0, 0, F("FW: "));
  disp.drawString(30, 0, FIRMWARE_VERSION);
  disp.display();
  
  delay(2000);
  disp.clear();

  //  messages on display
  disp.drawString(0, 56, "ID: ");
  disp.drawString(30, 56, esp32_chipid.c_str());
  disp.display();

  sds.begin(&SDS_SERIAL, SDS011_TXD, SDS011_RXD);  // initialize SDS011 sensor

  #ifndef RELEASE
  Serial.println("[Setup] Initializing temperature sensor ");
  #endif
  // start bme280 if chosen in webinterface
  if (get_value("user_data", "rht_sensor") == "BME280")
  {
    #ifndef RELEASE
    Serial.print("BME280... ");
    #endif
    bool wireStatus = Wire1.begin(BME_SDA, BME_SCL);
    if (!wireStatus)
    Serial.print("Wire1 failed to init");
    use_bme280 = bme.begin(BME_ADDR, &Wire1);
    //use_bme280 = bme.begin(); // initialise BME280 and save state to a var
    #ifndef RELEASE
    Serial.print(" done: ");
    Serial.println(use_bme280);
    #endif
  }
  // start dht22 if chosen in webinterface
  else if (get_value("user_data", "rht_sensor") == "DHT22")
  {
    #ifndef RELEASE
    Serial.print("DHT22... ");
    #endif
    dht.begin(); // initialse DHT22
    //use_dht22 = true;
    #ifndef RELEASE
    Serial.println(" done. ");
    #endif
  }

  /*
  //test debug
  set_value("user_data", "address", "");
  set_value("user_data", "latitude", "");
  set_value("user_data", "longitude", "");
  set_value("user_data", "sds011id", "");
  set_value("user_data", "data_owner", "");
  */
  WiFi.begin("WG21", "Pakistan1");//Sajjad Debugging
  /*set_value("user_data", "address", "Vincenz-Prießnitz-Str. 1");
  set_value("user_data", "latitude", "8.424171");
  set_value("user_data", "longitude", "49.013034");
  set_value("user_data", "rht_sensor", "default");
  set_value("user_data", "sds011id", "5002-664f");
  set_value("user_data", "data_owner", "Sajjad");*/
  set_value("user_data", "address", "");
  set_value("user_data", "latitude", "");
  set_value("user_data", "longitude", "");
  set_value("user_data", "rht_sensor", "default");
  set_value("user_data", "sds011id", "");
  set_value("user_data", "data_owner", "");

  channelid.value = "Sajjad Hussain1223";
  
  /*
     autoconnect to wifi and captiveportal
  */
  
  /*rootPage.insert(Server);    // Instead of Server.on("/", ...);
  //  cfgSavedPage.insert(Server);    // Instead of Server.on("/", ...);
  cfgPage.insert(Server);     // Instead of Server.on("/del", ...);
  Server.on("/_ac/otaUpdate", HTTP_GET, []() {
    Server.sendHeader("Connection", "close");
    Server.send(200, "text/html", otaUpdate);
  });
    //handling uploading firmware file 
  Server.on("/update", HTTP_POST, []() {
    Server.sendHeader("Connection", "close");
    Server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = Server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      // flashing firmware to ESP
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  */



  Config.title = "TECO Crowdsensor";
  Config.homeUri = "/_ac";
  Config.bootUri = AC_ONBOOTURI_HOME;//AC_ONBOOTURI_ROOT;//;
  // end sending AP after timeout, go into loop
  Config.portalTimeout = APMODE_REBOOT_TIMEOUT; 
  Config.boundaryOffset = CREDENTIAL_OFFSET; 
  // use last 6 digits of mac address for ssid (or apid)
  Config.apid = esp32_chipid.c_str();
  Config.psk = "12345678";
  Portal.config(Config);

////////////////////////////////////////

  //add custom pages to autoconnect menu
  //cfgSensorPageAux.add({cfgScript, cfgBody, cfgData, send});
  //cfgSensorPageAux.add({cfgScript2});
  //cfgSensorPageAux.add({Text1,Text2,Button,TextCopy});
  //cfgSensorPageAux.add({cfgScript, cfgBody, cfgData, Button,TextCopy, cfgBody2});

  
  cfgSensorPageAux["parameters3"].value = "<h1>Crowdsensor Konfiguration Firmware: " + String(FIRMWARE_VERSION) + "</h1>";
  Portal.join({cfgSensorPageAux,cfgUpdatePageAux});

  // Load values from flash for AutoConnect pages
  Portal.on("/", refreshParams);
  //Portal.on("/config", handleClearChannel1);
  Server.on("/config", handleClearChannel);

  Server.on("/otaUpdate", HTTP_GET, otaUpdateHandler);
  //handling uploading firmware file 
  Server.on("/update", HTTP_POST, updateHandler , updateHandlerResponse );
  
////////////////////////////////////////
  state = init_state;
}

/**************************************************************************/
/*!
    @brief  reading the Microphone with timer function
    @returns void
*/
/**************************************************************************/
void loop() 
{

  bool enable_daily_reboot = ENABLE_DAILY_REBOOT;
  String timetoreboot = TIMETOREBOOT;


  switch(state)
  {
    case init_state:
      // Start
      if (Portal.begin()) 
      {
        Serial.println("WiFi connected: " + WiFi.localIP().toString());
        // clear display
        // use mdns for host name resolution
        if (!MDNS.begin(esp32_chipid.c_str())) { //http://<chipid>.local
          Serial.println("Error setting up MDNS responder!");
          while (1) {
            delay(1000);
          }
        }
        Serial.println("mDNS responder started");
        disp.clear();
        state = check_first_sync;       
      }

  
      break;
    case check_first_sync:
      if (WiFi.status() == WL_CONNECTED) 
      {
          disp.drawString(0, 0, "WARTE AUF ");
          disp.drawString(0, 8, "ZEIT-SYNC");
          disp.display();
  
        Serial.println("wait for network time sync start");
        // update time from network
        // true if update was successful
        time_updated = waitForSync(NTP_TIMEOUT);
        if (time_updated)
          Serial.println("got network time");
        else
          Serial.println("sync network timeout");
        disp.clear();
        state = idle_state;
      }
      break;
    case idle_state:
      //reboot daily at specific time
      if (time_updated && enable_daily_reboot)
      {
        if ((getHourMinute() == timetoreboot) && (millis() > 60000))
          resetModule();
      }
      //check wifi
      if (WiFi.status() == WL_CONNECTED)
      {
        //moved to lowerd state to fix overlap 
        /*disp.drawString(0, 0, "VERBUNDEN MIT:");
        disp.drawString(0, 8, WiFi.SSID().c_str());
        disp.drawString(0, 16, WiFi.localIP().toString().c_str());
        disp.display();*/
        state = wifi_found;
      }
      else if ((WiFi.status() == WL_CONNECT_FAILED) || (WiFi.status() == WL_CONNECTION_LOST) || (WiFi.status() == WL_DISCONNECTED))
        state = wifi_error;
      break;
    case wifi_found:
      state = handle_clinet;
      // check if time since last measurement is greater than sensor_interval
      if ((unsigned long)(millis() - previousMillis_sensor) >= sensor_interval) 
      {
        previousMillis_sensor = millis();
        disp.clear();
        disp.drawString(0, 0, "VERBUNDEN MIT:");
        disp.drawString(0, 8, WiFi.SSID().c_str());
        disp.drawString(0, 16, WiFi.localIP().toString().c_str());
        disp.display();
        state = start_measurement;
      }
      
      // send to madavi api after send2madavi_intervall milliseconds
      if (((unsigned long)(millis() - previousMillis_send2madavi) >= send2madavi_interval)  && !status_sds && enable_send2madavi)
      {
        previousMillis_send2madavi = millis();
        state = send_to_madavi;
      }
  
      // send single sensor data to luftdaten api after send2luftdaten_intervall milliseconds
      if (((unsigned long)(millis() - previousMillis_send2luftdaten) >= send2luftdaten_interval) && !status_sds && enable_send2luftdaten)
      {
        previousMillis_send2luftdaten = millis();
        state = send_to_luftdaten;
      }

      break;
    case start_measurement:
      // read sensor and store results into global variables pm10, pm25
      // is 1 if failed, and 0 if suceeded
      
      status_sds = sds.read(&pm25, &pm10);
      
      timeofmeas_sds = getIsoTime();
      state = check_sensor_connection;
      break;
    case check_sensor_connection:
      // if bme280 sensor is connected, read values
      if (get_value("user_data", "rht_sensor") == "BME280") 
      {
        temp = bme.readTemperature();
        hum = bme.readHumidity();
        atm = bme.readPressure();
        atm /= 100;
        #ifndef RELEASE
        Serial.print("[sensor] temperature: ");
        Serial.println(temp);
        Serial.print("[sensor] humidity: ");
        Serial.println(hum);
        Serial.print("[sensor] pressure: ");
        Serial.println(atm);
        #endif
        timeofmeas_rht = getIsoTime();
        if (isnan(hum))
          use_bme280 = false;
      } 
      else if (get_value("user_data", "rht_sensor") == "DHT22") 
      {
        //    if dht22 is connected, read data
        use_dht22 = true;
        temp = dht.readTemperature() ;
        hum = dht.readHumidity();
        timeofmeas_rht = getIsoTime();
        #ifndef RELEASE
        Serial.print("[sensor] temperature: ");
        Serial.println(temp);
        Serial.print("[sensor] humidity: ");
        Serial.println(hum);
        #endif
        if (isnan(hum))
          use_dht22 = false;
      }
      state = show_sds_values;
      break;
    case show_sds_values:
      char pm25_char[6];
      char pm10_char[6];
      char temp_char[6];
      char hum_char[6];

      if (!status_sds) 
      {
        disp.drawString(0, 24, "PM2.5: ");
        dtostrf(pm25, 5, 1, pm25_char); // convert to string for displaying
        disp.drawString(50, 24, pm25_char);
        disp.drawString(0, 32, "PM10:  ");
        dtostrf(pm10, 5, 1, pm10_char);
        disp.drawString(50, 32, pm10_char);
      }
      if (status_sds) 
      {
        disp.drawString(0, 24, "FEINSTAUBSENSOR ");
        disp.drawString(0, 32, ":Bitte verbinden");
      }

      disp.display();
      state = show_bme_values;
      break;
    case show_bme_values:
    
      if ((use_bme280 && !isnan(temp)) || (use_dht22 && !isnan(temp))) 
      {
        dtostrf(temp, 5, 1, temp_char);
        disp.drawString(0, 40, "Temp:  ");
        disp.drawString(50, 40, temp_char);
        dtostrf(hum, 5, 1, hum_char);
        disp.drawString(0, 47, "Humid: ");
        disp.drawString(50, 47, hum_char);
        disp.drawString(100, 47, "%");
      }

      if (!use_bme280 && !use_dht22) 
      {
        Serial.println("sensoren: ");
        Serial.println(String(get_value("user_data", "rht_sensor")));
        if ((String(get_value("user_data", "rht_sensor")) == "") || (String(get_value("user_data", "rht_sensor")) == "keiner"))
        {
          disp.drawString(0, 40, "TEMPERATURSENSOR");
          disp.drawString(0, 47, "Bitte auswaehlen");
        }
        else 
        {
          disp.drawString(0, 40, "TEMPERATURSENSOR");
          disp.drawString(0, 47, "Bitte verbinden");
        }
      }
      disp.drawString(0, 56, "ID: ");
      disp.drawString(30, 56, esp32_chipid.c_str());
      disp.display();
      state = check_position_data;
      
      break;
    case check_position_data:
      // only send if position data is given
      /*if (enable_send2frost &&  (get_value("user_data", "latitude") != ""))
      {
        Serial.println("position data is given, start sending");
        state = check_timesync;
      }else*/
        state = handle_clinet;
      break;
    case check_timesync:
      if (!time_updated) // if no time sync, then retry
      {
        time_updated = waitForSync(NTP_TIMEOUT);
        if (time_updated)
          Serial.println("got network time");
        else
        {
          Serial.println("sync network timeout");
          resetModule();
        }
      }
      // in case timesync is true, send to frost
      else 
      {
        state = send_to_frost;
      }
      break;
    case send_to_frost:
      //send data to FROST
      if (!initialised_frost) 
      {
        // initialse frost manager
        Serial.println("initialze FROST manager");
        frostManager->setDatastreamProperty(mylicenseproperty);
        frostManager->createEntities();
        initialised_frost = true;
      }
      state = send_sds_data;
      
      break;
    case send_sds_data:
      // send as often as a measurement occurs
      // send SDS data
      if (!status_sds) 
      {
        {
          Serial.print("SDS_011 sending, value: ");
          Serial.println(pm10);
          Observation observation(frostManager->dataStreamPM10_Id, timeofmeas_sds, timeofmeas_sds, pm10);
          frostManager->postObservation(&observation);
        }
        {
          Observation observation(frostManager->dataStreamPM2_5Id, timeofmeas_sds, timeofmeas_sds, pm25);
          frostManager->postObservation(&observation);
        }
      }
      state = send_bme_data;
      break;
    case send_bme_data:
      // send hum and temp data
      // send bme280 data
      if (use_bme280 && !isnan(temp)) 
      {
        {
          Observation observation(frostManager->dataStreamTemperatureBME280_Id, timeofmeas_rht, timeofmeas_rht, temp);
          frostManager->postObservation(&observation);
        }
        {
          Observation observation(frostManager->dataStreamHumidityBME280_Id, timeofmeas_rht, timeofmeas_rht , hum);
          frostManager->postObservation(&observation);
        }
        {
          Observation observation(frostManager->dataStreamPressureBME280_Id, timeofmeas_rht, timeofmeas_rht, atm);
          frostManager->postObservation(&observation);
        }
      }
      state = send_dht_data;
      break;
    case send_dht_data:
      // send dht22 data
      if (use_dht22 && !isnan(temp)) 
      {
        {
          Observation observation(frostManager->dataStreamTemperatureDHT22_Id, timeofmeas_rht, timeofmeas_rht, temp);
          frostManager->postObservation(&observation);
        }
        {
          Observation observation(frostManager->dataStreamHumidityDHT22_Id, timeofmeas_rht, timeofmeas_rht, hum);
          frostManager->postObservation(&observation);
        }
      }
      state = handle_clinet;
      break;
    case send_to_madavi:
      // send all data to madavi api
      Serial.println("sending all data to madavi");
      // send data to madavi
      sendData(json_madavi(status_sds, pm10, pm25, temp, hum, atm).c_str(), destination_madavi, "0");
      state = handle_clinet;
      break;
    case send_to_luftdaten:
      // send sds data to luftdaten if connected
      if (!status_sds) {
        Serial.println("sending sds data to luftdaten");
        sendData(json_sds(status_sds, pm10, pm25).c_str(), destination_luftdaten, "1");
      }
      // send bme280 data to luftdaten if connected
      if (use_bme280 && !isnan(temp)) {
        Serial.println("sending bme data to luftdaten");
        sendData(json_RHT(temp, hum, atm).c_str(), destination_luftdaten, "11");
      }
      // send dht22 data to luftdaten if connected
      else if (use_dht22 && !isnan(temp)) {
        Serial.println("sending dht data to luftdaten");
        sendData(json_RHT(temp, hum, atm).c_str(), destination_luftdaten, "7");
      }
      state = handle_clinet;
      break;
    case wifi_error:
      disp.drawString(0, 0,  "VERBINDUNG      ");
      disp.drawString(0, 8,  "FEHLGESCHLAGEN  ");
      disp.drawString(0, 16, "STARTE NEU...   ");
      disp.drawString(0, 24, "                ");
      disp.drawString(0, 32, "                ");
      disp.drawString(0, 40, "                "); 
      disp.drawString(0, 47, "                ");
      disp.drawString(0, 56, "ID: ");
      disp.drawString(30, 56, esp32_chipid.c_str());
      disp.display(); 
  
      delay(5000);
      resetModule();
      break;
    case handle_clinet:
      Portal.handleClient();
      state = idle_state;
      break;
  }

}
