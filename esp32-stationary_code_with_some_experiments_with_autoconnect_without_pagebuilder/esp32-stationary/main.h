/**
 * \file main.h
 * \brief This the main page for the SMARTAQNET which is the core of the project. 
 * \mainpage Smart Air Quality Network (SMARTAQNET)
 * 
 * \section intro Introduction
 * SMARTAQNET project is aimed at development of a close-meshed network of local fine-dust data, which can be fed and used well by general public.
 * Further details can be checked at: http://www.smartaq.net.\n
 * This script uses ESP32 HELTEC Board, SDS011 Sensor and a BME280 sensor or a DHT22 sensor.  After power on, it opens a Wifi AP, default Password is 12345678.
 * The Wifi AP can be searched using the ID shown on the OLED display. A Captive Portal opens after connection to this AP with Smartphone. Using this portal connect to any
 * of the available Wifi SSIDs. Then one can access the configuration page using http://[localIP]/ or using http://[SSD-ID].local/ e.g., http://192.168.0.39 or http://181712.local.
 * The location address, the thermal sensor type, ID of dust sensor, owner of the data etc. can be configure using this page.\n
 * The project work-flow can be see in the <a href="StatemachineFlow.pdf" target="_blank"><b>flowchart</b></a>.\n
 * Have a look at the following constants to configure them accordingly:
 * - in the file myAutoConnect.h 
 *  - WIFI_REBOOT_TIMEOUT set Timeout to search WiFi already configured
 * - in the file sensor.h
 *  - DHT_PIN 21, set DHT22 sensor communication pin
 *  - BME_ADDR  0x77, set BME280 I2C Address 0x77 for Red, 0x76 for Purple 
 *  - BME_SDA, set BME280 I2C SDA pin
 *  - BME_SCL, set BME280 I2C SCL pin
 *  - SDS011_TXD, set SDS011 TXD pin is connected at RXD of Serial2 Object
 *  - SDS011_RXD, set SDS011 RXD pin is connected at TXD of Serial2 Object (no need to physically connect)
 * - in the file main.h
 *  - ENABLE_SEND2FROST, set switch for sending to FROST teco server
 *  - SENSOR_INTERVAL, set  intervall of reading sensor data in ms
 *  - ENABLE_SEND2MADAVI, set switch for sending to madavi api
 *  - ENABLE_SEND2LUFTDATEN, set switch for sending to luftdaten api
 *  - SEND2MADAVI_INTERVAL, set intervall of sending to madavi api in ms 
 *  - SEND2LUFTDATEN_INTERVAL, set intervall of sending to madavi api in ms 
 *  - ENABLE_DAILY_REBOOT, set enable daily reboot at specific time
 *  - TIMETOREBOOT, set time when to do daily reboot (hour of time of day for reboot goes here) //TODO set proper time
 *  - ENABLE_APMODE_REBOOT, set if true, automatic reboot after REBOOT_TIMEOUT is enabled
 *  - APMODE_REBOOT_TIMEOUT, set intervall of auto reboot while in access point mode (config mode) in ms (36000000ms is 10min). autoreboot while in ap mode only occurs if no device is connected to the esp32
 *  - NTP_TIMEOUT, set Timeout for NTP timeout (s) in waitForSync()
 *  - CREDENTIAL_OFFSET, Specified the offset if the user data exists.
 *  - RST, set Reset PIN of OLED Module
 *  - OLED_ADDR, The I2C address of the SSD1306 OLDE 
 *  - OLED_SDA, The SDA pin of the SSD1306 OLDE 
 *  - OLED_SCL, The SCL pin of the SSD1306 OLDE 
 * \n
 * 
 * \section  Pre-requisites
 * -# Arrange the required \ref hw_sec and carry out the \ref wiring.
 * -# Install the the \ref arduinoide
 * -# Install the \ref boardlib
 * -# Install the recommended \ref lib
 * 
 * \section hw_sec Hardware
 * It consists of ESP32 microcontrollers, which is "HelTec WiFi Kit 32" ----> https://heltec.org/project/wifi-kit-32/
 * Following board, hardware and libraries are used to develop this project
 * 
 * \subsection peri Peripherals
 * -# OLED Built-in OLED Display ----> https://heltec.org/project/wifi-kit-32/
 * -# SDS011 Sensor ----> https://learn.watterott.com/sensors/sds011/ 
 * -# BME820 Sensor for Humidity and Temperature ----> https://www.ebay.de/itm/3-3V-Bosch-BME280-Barometer-Luftdruck-Luftfeuchte-Sensor-Modul-BMP280-SPI-I2C-/263548911232 or https://eckstein-shop.de/SparkFun-Barometric-Pressure-Sensor-Breakout-MPL115A1?curr=EUR&gclid=Cj0KCQjww7HsBRDkARIsAARsIT6QsuuszbyaQ4r1DLuYTOT5pgwMfpIhRTtwQKAA2vtHkuHGrBZa9JQaAo-gEALw_wcB
 * -# DHT22 Sensor for Temperature and Humidity ----> https://www.sparkfun.com/products/10167 
 * 
 * 
 * \subsection wiring  Wirings
 * -# OLED Display\n
 *  The display is built-in already with the ESP32 board through I2C. The I2C address is 0x3c, SDA pin is 4, SCL pin is 15, and RST pin is 16. They are already connected. The custom fonts are generated using
 *  online Font Generatore from ----> http://oleddisplay.squix.ch/#/home
 * -# SDS011 Dust Sensor\n
 * One has to:
 *  - connect TXD of Dust Sensor SDS011 to Pin 13 of ESP32 (ESP-RX)
 *  - connect 5V of Dust Sensor SDS011 to Pin 5V of ESP32
 *  - connect GND of Dust Sensor SDS011 to Pin GND of ESP32
 * -# BME820 Sensor for Humidity and Temperature\n
 *  - connect SCK of BME280 sensor to Pin 22 of ESP32 (ESP-SCL)
 *  - connect SDA of BME280 sensor to Pin 21 of ESP32 (ESP-SDA) 
 *  - connect VCC of BME280 sensor to 3V3 of ESP32 
 *  - connect GND of BME280 sensor to GND of ESP32
 * -# DHT22 Sensor for Temperature and Humidity\n
 * Look at the sensor from the front (grid shows in your direction), put the pins facing down, start counting the pins from left (pin1) to right (pin4). pin 3 is not connected.\n One has to:
 *  - Connect pin 1 of DHT22 sensor (on the left) to 3V3 of ESP32
 *  - Connect pin 2 of DHT22 sensor to Pin 21 of ESP32 (data)
 *  - Connect pin 4 of DHT22 sensor (on the right) to GND of ESP32
 *  - Connect a resistor in range 4.7k to 10k from pin 2 (data) to pin 1 (power) of the sensor
 * 
 * \subsection req Requirements    
 * 
 * \subsection arduinoide Arduino IDE
 * The current upstream at the 1.8 level or later is needed. Please install from the official Arduino IDE download page. This step is not required if you already have a modern version.
 * \subsection boardlib ESP32 Arduino core
 * Also, to apply AutoConnect to ESP32, the arduino-esp32 core provided by Espressif is needed. Stable 1.0.1 or required and the latest release is recommended.
 * Install third-party platform using the Boards Manager of Arduino IDE. You can add multiple URLs into Additional Board Manager URLs field, separating them with commas. Package URL is https://dl.espressif.com/dl/package_esp32_index.json for ESP32.
 * 
 * \subsection lib Additional Libraries (Required)
 *  -# [till FIRMWARE_VERSION   "0.8.6"]
 *    - Adafruit Unified Sensor by Adafruit v1.0.3 (https://github.com/adafruit/Adafruit_Sensor)    
 *    - Adafruit BME280 Library by Adafruit v1.0.8 (https://github.com/adafruit/Adafruit_BME280_Library)    
 *    - ArduinoJson by Benuit Blanchon v 5.13.5 (https://arduinojson.org/?utm_source=meta&utm_medium=library.properties) (IMPORTANT: ArduinoIDE installs the newest beta version (v6), but it doesnt work with the newest version.)    
 *    - DHT sensor library by Adafruit v1.3.4 (https://github.com/adafruit/DHT-sensor-library)    
 *    - PageBuilder v1.3.4 by Hieromon Ikasamo (https://github.com/Hieromon/PageBuilder)    
 *    - eztime by Rop Gonggrijp v0.7.10 (https://github.com/ropg/ezTime)    
 *    - u8g2 by oliver v2.25.10 (https://github.com/olikraus/u8g2)  
 *    - Autoconnect lib (is already in src folder, no need to install) [MODIFIED] [till FIRMWARE_VERSION   "0.8.6 pre"]
 *    - SDS011 sensor Library by R. Zschiegner (already in src folder, no need to install) [MODIFIED] [till FIRMWARE_VERSION   "0.8.6 pre"]
 *  -# [after FIRMWARE_VERSION   "0.8.6.1"]
 *    - ESP8266 QRcode by Anun Panya v2.0.0 (https://github.com/anunpanya/ESP8266_QRcode)
 *    - ESP8266 and ESP32 Oled Driver for SSD1306 display by Daniel Eichhorn, Fabrice Weinberg v4.0.0 (https://github.com/ThingPulse/esp8266-oled-ssd1306)
 *    - AutoConnect by Hieromon Ikasamo v1.0.2 (https://github.com/Hieromon/AutoConnect.git)
 *    - ArduinoJson by Benoit Blanchon v6.12.0 (https://github.com/bblanchon/ArduinoJson.git)
 *             
 * \section changelog  Change Log
 * \subsection changelog2 FIRMWARE_VERSION 0.8.6.1 onwards
 * - [0.8.6.1]  Sept. 19, 2019
 *  - cleaning all the unnessary comments, defining HW pins constants, addresses etc and documenting them. (Sajjad)
 *  - moved to JSON6 (by Jan)
 *  - fixed graphic OLED overlapping bugs because it don't have clearline() function and it clears whole display (Sajjad)
 *  - using BME280 RED/PURPLE by just changing address in bme(address, sda, scl) (Sajjad)
 *  - fixed BME280 not found problem by using another TwoWire instance Wire1 (Sajjad)
 *  - started doxygen documentations (Sajjad)
 *  - graphic based oled with qr support (Sajjad)
 *  - converted to state machine (Sajjad)
 *  - inherited AutoConnect to use custom code (Sajjad)
 *  - added NTP sync timeout (Sajjad)
 *             
 * \subsection changelog1 FIRMWARE_VERSION 0.8.6 and older
 * change history (since version 0.8)\n    
 * - [0.8.1]\n
 *  bugfix for coordinates: switched to coordinates[]={long, lat...}\n    
 * - [0.8.2]\n    
 *  added hack for HTTP patch for location\n
 *  removed all libs from src folder that are used in its original state and included the libs directly in arduino\n
 *  updated SDS011 lib to newest version\n
 *  changed display font\n
 *  added over the air update support\n
 *  added mDNS: now esp32.local can be typed into browser to connect to esp32\n
 * - [0.8.3]\n
 *  bugfix for error that no reboot occured after timeout in ap mode\n
 *  bugfix for wrong display message if no RH T sensor is configured\n
 *  bugfix for wrong display message if Wifi is searched\n
 *  improved display messages\n
 *  bugfix for not working geocode button\n
 * - [0.8.4]\n
 *  added properties and webinterface inputs for data license\n
 *  geocoding button does not work again, previous bugfix did not work\n
 *  [...]  \n
 * - [0.8.5]\n
 *  (hopefully) permanent bugfix for not working geocode button\n
 *  added license url to webinterface \n
 *  added RELEASE macro that can be used to switch between development and release versions (switches server addresses and supressses some serial output)\n
 *  display shows FW version for 2 sec on startup \n
 *  webinterface shows map to pick location\n
 *  added reboot after saving configuration, so that thing and datastreams are patched in new setup()\n
 *  \n
 *    
 *
 * \section trouble Troubleshooting
 * if the storage space is too small when compiling, change it in arduino IDE:\n
 * Tools -> board -> select ESP32 DEV Module\n
 * Tools -> partition scheme -> No OTA (large APP)\n
 *    -# NVS:
 *    ESP32 key value store with Preferences library.
 *    uses the Preferences library to store key value pairs in
 *    non-volatile storage on ESP32 processor.
 *    created for arduino-esp32 09 Jul 2018
 *    by Felix Ernst (ernst(at)teco.edu)
 *    -# Autoconnect:
 *    allows to connect to the ESP via Wifi and then change to AP to which the ESP connects
 *    default ssid of ESP: esp32ap,  default wifi-pw: 12345678
 *    -# Auto reboot:
 *    The device reboots after a certain time (see watchdog.h)
 *
 *\section author Author(s)
 * - Felix Ernst, 
 * - Jan Formanek, 
 * - Sajjad Hussain, 
 * - Matthias Budde, 
 *
 * The team at TECO (www.teco.edu) and at Chair for Pervasive Computing Systems (https://pcs.tm.kit.edu).
 *
 *
 * \section license License
 * This code belongs to TECO www.teco.edu and must be reffered when used. BSD license, all text here must be included in any redistribution.
 * \subsection ack Acknowledgments
 * The following libraries were used:\n
 * - Espressif Systems: Licenses and Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
 * - ArduinoJson is licensed under the MIT License. https://arduinojson.org/
 * - Adafruit BME280 is licensed under Adafruit. https://adafruit.com/
 * - DHT sensor is licensed under Adafruit. https://adafruit.com/
 * - AutoConnect is licensed under the MIT License. https://github.com/Hieromon/AutoConnect
 * - ESP8266 and ESP32 Oled Driver for SSD1306 display is licensed under the MIT License. http://blog.squix.ch
 * - ESP8266 QRcode is licensed under the MIT License. http://blog.squix.ch
 * - ezTime is licensed under the MIT License. https://github.com/ropg/ezTime
 * - PageBuilder is licensed under the MIT License. https://github.com/Hieromon/PageBuilder
 * - U8g2 is licensed under the MIT License. https://github.com/olikraus/u8g2
 * 
 * 
 * \subsection mit MIT License
 * 
 *        Copyright (c) 2017-2019 Hieromon Ikasamo
 * 
 *        Permission is hereby granted, free of charge, to any person obtaining a copy
 *        of this software and associated documentation files (the "Software"), to deal
 *        in the Software without restriction, including without limitation the rights
 *        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *        copies of the Software, and to permit persons to whom the Software is
 *        furnished to do so, subject to the following conditions:
 * 
 *        The above copyright notice and this permission notice shall be included in all
 *        copies or substantial portions of the Software.
 * 
 *        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *        SOFTWARE.
 * 
 * 
 * \subsection u8gLic U8g2lib License
 * 
 *        The U8g2lib code (http://code.google.com/p/u8g2/) is licensed under the terms of 
 *        the new-bsd license (two-clause bsd license).
 *        See also: http://www.opensource.org/licenses/bsd-license.php
 * 
 *        The repository and optionally the releases contain icons, which are
 *        derived from the WPZOOM Developer Icon Set:
 *        http://www.wpzoom.com/wpzoom/new-freebie-wpzoom-developer-icon-set-154-free-icons/
 *        WPZOOM Developer Icon Set by WPZOOM is licensed under a Creative Commons 
 *        Attribution-ShareAlike 3.0 Unported License.
 * 
 *        Fonts are licensed under different conditions.
 *        See  https://github.com/olikraus/u8g2/wiki/fntgrp
 *        for detailed information on the licensing conditions for each font. 
 * 
 * 
 * \subsection EspressifLic Espressif Systems License
 * 
 *        Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
 *        
 *        Licensed under the Apache License, Version 2.0 (the "License");    
 *        you may not use this file except in compliance with the License.    
 *        You may obtain a copy of the License at    
 *        http://www.apache.org/licenses/LICENSE-2.0    
 *        Unless required by applicable law or agreed to in writing, software    
 *        distributed under the License is distributed on an "AS IS" BASIS,    
 *        WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    
 *        See the License for the specific language governing permissions and    
 *        limitations under the License.
 *        
 *        
 *\date 19.08.2019
 */



//******************************************************** constants ***************************

/// Debugging, uncomment to switch servers to release version, enable sending to madavi and luftdaten.info, and supress some debug output
//#define RELEASE 
/// firmware version, (for intermediate versions being actively worked on, append "pre" to number, indicating that it is "not yet" the specified number)
#define FIRMWARE_VERSION "0.8.6.1 pre" 

/// switch for sending to FROST teco server
#define ENABLE_SEND2FROST true 
/// intervall of reading sensor data in ms
#define SENSOR_INTERVAL 10000 
#ifdef RELEASE
  /// switch for sending to madavi api
  #define ENABLE_SEND2MADAVI true  
  /// switch for sending to luftdaten api
  #define ENABLE_SEND2LUFTDATEN true  
#else
  #define ENABLE_SEND2MADAVI false 
  #define ENABLE_SEND2LUFTDATEN false
#endif
/// intervall of sending to madavi api in ms 
#define SEND2MADAVI_INTERVAL 145000
/// intervall of sending to madavi api in ms 
#define SEND2LUFTDATEN_INTERVAL 145000
///enable daily reboot at specific time
#define ENABLE_DAILY_REBOOT true 
/// set time when to do daily reboot (hour of time of day for reboot goes here) //TODO set proper time
#define TIMETOREBOOT "0400" 
/// if true, automatic reboot after REBOOT_TIMEOUT is enabled
#define ENABLE_APMODE_REBOOT true 
/// intervall of auto reboot while in access point mode (config mode) in ms (36000000ms is 10min). autoreboot while in ap mode only occurs if no device is connected to the esp32
#define APMODE_REBOOT_TIMEOUT 3600000 
/// Timeout for NTP timeout (s) in waitForSync()
#define NTP_TIMEOUT 60 

/// Specified the offset if the user data exists.
// The following two lines define the boundalyOffset value to be supplied to
// AutoConnectConfig respectively. It may be necessary to adjust the value
// accordingly to the actual situation.

#define CREDENTIAL_OFFSET 0
//#define CREDENTIAL_OFFSET 64


/// Reset PIN of OLED Module
#define RST 16
/// The I2C address of the SSD1306 OLDE 
#define OLED_ADDR 0x3c
/// The SDA pin of the SSD1306 OLDE 
#define OLED_SDA 4
/// The SCL pin of the SSD1306 OLDE 
#define OLED_SCL 15

/// state machine initial state
#define init_state          1
/// state machine check NTP sync state
#define check_first_sync      2
/// state machine idle state
#define idle_state          3
/// state machine WiFi found state
#define wifi_found        4
/// state machine start measurement state
#define start_measurement     5
/// state machine check sensor selection state
#define check_sensor_connection   6
/// state machine show valid SDS values state
#define show_sds_values       7
/// state machine show valid BME values state
#define show_bme_values       8
/// state machine check if position data is given state
#define check_position_data     9
/// state machine check NTP sync again state
#define check_timesync        10
/// state machine check if frost is initialized state
#define send_to_frost       11
/// state machine send sds data state
#define send_sds_data       12
/// state machine send bme data state
#define send_bme_data       13
/// state machine send dht data state
#define send_dht_data       14
/// state machine send madavi data state
#define send_to_madavi        15
/// state machine send luftdaten data state
#define send_to_luftdaten     16
/// state machine WiFi error state
#define wifi_error          17
/// state machine handle client state
#define handle_clinet       18

//******************************************************** includes ***************************

#include "src/lib/SDS011/SDS011.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
//#include "src/lib/AutoConnect/src/AutoConnect.h"
//#include "src/lib/AutoConnect/src/AutoConnectCredential.h"
#include <AutoConnect.h>
#include <PageBuilder.h>
#include <Wire.h>
#include "SSD1306.h"
#include <qrcode.h>
#include <time.h>
#include <ezTime.h>
#include <HTTPClient.h>
#include "nvs_storage.h"
#include "sensor.h"
#include "timer_definitions.h"
#include "send_data.h"
#include "create_json.h"
#include "src/esp32_thingsapi/src/librest/rest_entities.h"
#include "src/esp32_thingsapi/src/workshop_instances/workshop_entities.h"
#include "src/esp32_thingsapi/src/librest/Frost_Server.h"
#include "src/esp32_thingsapi/src/librest/FrostManager.h"
#include "myAutoConnect.h"//inherited the AutoConnect class and overridden the begin() function.


//******************************************************** variables ***************************

/// state machine tracking variable
unsigned char state;
/// host to send data madavi API
//char destination_madavi[] = "http://192.168.12.67/data.php"; //TODO set proper url
//char destination_luftdaten[] = "http://192.168.12.67/data.php"; //TODO set proper url
char destination_madavi[] = "https://api-rrd.madavi.de/data.php";
/// host to send data luftdaten API
char destination_luftdaten[] = "https://api.luftdaten.info/v1/push-sensor-data/";


// do not change this vars
/// used to store mac address
byte mac[6];
/// Chip ID based on MAC
String esp32_chipid;
/// tracking if BME280 sensor is selected
bool use_bme280 = false;
/// tracking if DHT22 sensor is selected
bool use_dht22 = false;
/// if frost manager is initialized
bool initialised_frost = false;
/// if wifi is connected
bool wifi_connected;
/// switch for sending to FROST teco server
bool enable_send2frost = ENABLE_SEND2FROST;
/// switch for sending to madavi api
bool enable_send2madavi = ENABLE_SEND2MADAVI;
/// switch for sending to luftdaten api
bool enable_send2luftdaten = ENABLE_SEND2LUFTDATEN;
/// if NTP time is synced
bool time_updated;


/// initialise the OLED which is used. OLED Module Object with I2C
SSD1306 disp(OLED_ADDR, OLED_SDA, OLED_SCL);
/// QRCode Object
QRcode qrcode (&disp);
/// initialise udp for network time
WiFiUDP ntpUDP;
/// initialise Webserver
WebServer Server; 
/// initialise AutoConnect
//AutoConnect Portal(Server); 
myAutoConnect Portal(Server);
/// initialise AutoConnectConfig
AutoConnectConfig  Config;
/// url for public domain
static String data_url_cc0 = "https://creativecommons.org/publicdomain/zero/1.0/deed.de";
/// url for license property
static String data_url_ccby = "https://creativecommons.org/licenses/by/4.0/deed.de";

#define data1_url_cc0 "https://creativecommons.org/publicdomain/zero/1.0/deed.de"
#define data1_url_ccby "https://creativecommons.org/licenses/by/4.0/deed.de"


/// Page for OTA Programming
const char* otaUpdate = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

/// Page for crowdsensor WiFi Connection
static const char PROGMEM html[] = "<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"  <meta charset=\"UTF-8\" name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"  <style>\n"
"  html {\n"
"  font-family:Helvetica,Arial,sans-serif;\n"
"  -ms-text-size-adjust:100%;\n"
"  -webkit-text-size-adjust:100%;\n"
"  }\n"
"  .menu > a:link {\n"
"    position: absolute;\n"
"    display: inline-block;\n"
"    right: 12px;\n"
"    padding: 0 6px;\n"
"    text-decoration: none;\n"
"  }\n"
"  </style>\n"
"  <script src=\"http://www.openlayers.org/api/OpenLayers.js\"></script>\n"
"  <script>\n"
"    //global vars\n"
"    var latitude;\n"
"    var longitude;\n"
"    //var elevation;\n"
"    var address            = \"\";\n"
"    var wgsProjection      = new OpenLayers.Projection(\"EPSG:4326\");   // WGS 1984 Projection\n"
"    var mercatorProjection = new OpenLayers.Projection(\"EPSG:900913\"); // Spherical Mercator Projection\n"
"    var initialposition    = new OpenLayers.LonLat(8.424171,49.013034).transform(wgsProjection, mercatorProjection);\n"
"    var initialzoom        = 3;\n"
"    var position           = initialposition;\n"
"    var zoom               = 16;\n"
"    var mapnik             = new OpenLayers.Layer.OSM();\n"
"    var markers            = new OpenLayers.Layer.Markers(\"Markers\");\n"
"    var marker             = new OpenLayers.Marker(position);\n"
"    var touchNavigation    = new OpenLayers.Control.TouchNavigation({\n"
"                                 defaultClickOptions:{\n"
"                                     pixelTolerance: 10\n"
"                                 }\n"
"                             });\n"
"\n  "
"\n  "
"    // function to geocode: look up coordinates from address text field, populate lon and lat fields and update map \n"
"    function geolookup() {\n"
"      address = document.getElementById('address').value;\n"
"      var request = new XMLHttpRequest();\n"
"      var url = \"https://nominatim.openstreetmap.org/search/'\" + encodeURI(address) + \"'?format=json&limit=1\";\n"
"      request.open('GET', url, true);\n"
"      request.onload = function() {\n"
"        if (this.status >= 200 && this.status < 300) {\n"
"          var response = JSON.parse(this.responseText);\n"
"          if (response[0]) {\n"
"            latitude = Number(response[0].lat).toFixed(6);\n"
"            longitude = Number(response[0].lon).toFixed(6);\n"
"            document.getElementById(\"longitude\").value = longitude;\n"
"            document.getElementById(\"latitude\").value = latitude;\n"
"            updateMarker();\n"
"          } else {\n"
"            alert(\"Keine Koordinaten gefunden, bitte Adress-Anfrage ändern.\");\n"
"          }\n"
"        } \n"
"      };\n"
"      request.send();\n"
"    }\n"
"\n  "
"    // function to \"move\" marker (avoiding marker.moveTo(), as it is an \"unofficial\" function)\n"
"    function replaceMarker() {\n"
"      markers.clearMarkers();\n"
"      position = new OpenLayers.LonLat(longitude,latitude).transform(wgsProjection, mercatorProjection);\n"
"      marker = new OpenLayers.Marker(position);\n"
"      markers.addMarker(marker);\n"
"    }\n"
"\n  "
"\n  "
"    // function to update marker based on inputs and center on map\n"
"    function updateMarker() {\n"
"      longitude = document.getElementById(\"longitude\").value;\n"
"      latitude = document.getElementById(\"latitude\").value;  \n"
"\n  "
"      if (latitude && longitude) {\n"
"        replaceMarker();\n"
"        map.setCenter(position, zoom);\n"
"      } else {\n"
"        markers.clearMarkers();\n"
"        map.setCenter(initialposition, initialzoom);\n"
"      }\n"
"    }\n"
"\n  "
"    // function to \"move\" marker on click in map\n"
"    OpenLayers.Util.extend(touchNavigation, {\n"
"      defaultClick: function(evnt) {\n"
"        var lonlat = map.getLonLatFromViewPortPx(evnt.xy);\n"
"\n"
"        latitude =  lonlat.transform(mercatorProjection, wgsProjection).lat.toFixed(6);\n"
"        longitude = lonlat.lon.toFixed(6);\n"
"\n"
"        document.getElementById(\"latitude\").value = latitude;\n"
"        document.getElementById(\"longitude\").value = longitude;\n"
"\n"
"        replaceMarker();\n"
"      }\n"
"    });\n"
"\n"
"    // function to unfocus current form element when enter key was pressed\n"
"    function unfocusForm() {\n"
"      if (event.key == 'Enter') {\n"
"        document.activeElement.blur();\n"
"        return false;\n"
"      } else {\n"
"        return true;\n"
"      }\n"
"    }\n"
"\n  "
"    // execute the following once all DOM elements are loaded\n"
"    document.addEventListener('DOMContentLoaded', function(){\n"
"\n  "
"      if(\"{{WIFICONNECTIVITY}}\"== \"true\"){\n"
"        // add geocoding button\n"
"        var findbutton = document.createElement(\"button\");\n"
"        findbutton.setAttribute(\"id\", \"geocode\");\n"
"        findbutton.setAttribute(\"type\", \"button\");\n"
"        findbutton.innerHTML = \"Suche Koordinaten\";\n"
"        findbutton.addEventListener (\"click\", function() {\n"
"          geolookup();\n"
"        });\n"
"        document.getElementById(\"pgeocode\").appendChild(findbutton);\n"
"\n  "
"        // init OpenLayers map\n"
"        map = new OpenLayers.Map(\"mapDiv\");\n"
"        map.addLayer(mapnik);\n"
"        map.addLayer(markers);\n"
"\n  "
"        updateMarker();\n"
"\n  "
"        // register function to \"move\" marker\n"
"        map.addControl(touchNavigation);   \n"
"        touchNavigation.activate();\n"
"      }\n"
"    });\n"
"  </script>\n"
"</head>\n"
"<body>\n"
"\n"
"<div class=\"menu\">{{AUTOCONNECT_MENU}}</div>\n"
"<h1 style=\"width: 90%\">Crowdsensor Konfiguration (Firmware {{FWVERSION}})</h1>\n"
"\n"
"  <div id=\"mapDiv\" style=\"height:250px\"></div>\n"
"  <h3>Standort des Sensors</h3>\n"
"  <form onsubmit=\"return confirm('Sind die Daten korrekt?');\" onkeydown=\"return unfocusForm()\" action=\"/config\" method=\"POST\">\n"
"  <label for \"address\"><b>Adresse</b>, Format: Straße Hausnummer, Stadt</label>\n"
"  <br />\n"
"  <input type=\"text\" name=\"address\" id=\"address\" maxlength=\"160\" placeholder=\"Adresse\" value=\"{{ADDRESS}}\">\n"
"  <br />\n"
"  <span id=\"pgeocode\" style=\"font-size: 75%;\"></span>\n"
"  <br />\n"
"  <label for=\"longitude\"><b>Länge</b>, Format: -3.703790 (für Europa im Bereich -15 bis 35):</label>\n"
"  <br />\n"
"  <input type=\"number\" name=\"longitude\" id=\"longitude\" min=\"-180\" max=\"180\" step=\"0.000001\" placeholder=\"z.B. -3.703790\" value=\"{{LON}}\" required onchange=\"updateMarker()\">\n"
"  <br />\n"
"  <br />\n"
"  <label for=\"latitude\"><b>Breite</b>, Format: 40.416775 (für Europa im Bereich 35 bis 75):</label>\n"
"  <br />\n"
"  <input type=\"number\" name=\"latitude\" id=\"latitude\" min=\"-90\" max=\"90\" step=\"0.000001\" placeholder=\"z.B. 40.416775\" value=\"{{LAT}}\" required onchange=\"updateMarker()\"> \n"
"  <br />\n"
"  <br />"
"  <h3>Sensoren</h3>\n"
"  <label for=\"rht_sensor\">Wähle den Temperatursensor aus, der benutzt werden soll</label>\n"
"  <br />\n"
"  <select name=\"rht_sensor\">\n"
"  <option value=\"{{RHTSENSOR}}\">{{RHTSENSOR}} (aktuell genutzt)</option>\n"
"  <option value=\"keiner\" default>(keiner)</option>\n"
"  <option value=\"BME280\">BME280</option>\n"
"  <option value=\"DHT22\">DHT22</option>\n"
"  </select>\n"
"  <br />\n"
"  <br />\n"
"  <label for=\"sds011id\"><b>ID des SDS011</b> Feinstaub-Sensors (XXXX-XXXX)</label>\n"
"  <br />\n"
"  <input type=\"text\"  name=\"sds011id\" maxlength=\"10\" placeholder=\"XXXX-XXXX\" value=\"{{SDS011ID}}\">\n"
"  <br />\n"
"  <br />\n"
"  <h3>Datenlizenz</h3>\n"
"  <label for=\"data_license\">Wähle die Lizenz aus, unter der die von deinem Sensor aufgenommenen Daten im SmartAQnet veröffentlicht werden sollen</label>\n"
"  <br />\n"
"  <select name=\"data_license\" id=\"license\" onchange=\"set_required()\">\n"
"  <option value=\"CC0 1.0\">CC0 1.0 Universell</option>\n"
"  <option value=\"CC BY 4.0\">CC BY 4.0 Namensnennung International</option>\n"
"  </select>\n"
"  <br />\n"
"  <span> Weitere Informationen: <a href=\"{{DATAURLCC0}}\" id=\"license_url\">{{DATAURLCC0}}</a></span>\n"
"  <script>\n"
"  function set_required() {\n"
"    var lic = document.getElementById('license').value;\n"
"      if (lic == \"CC BY 4.0\") {\n"
"        document.getElementById(\"owner\").required = true;\n"
"        document.getElementById(\"license_url\").innerHTML = \"{{DATAURLCCBY}}\";\n"
"        document.getElementById(\"license_url\").href = \"{{DATAURLCCBY}}\";\n"
"      } else {\n"
"        document.getElementById(\"owner\").required = false;\n"
"        document.getElementById(\"license_url\").innerHTML = \"{{DATAURLCC0}}\";\n"
"        document.getElementById(\"license_url\").href = \"{{DATAURLCC0}}\";\n"
"      }\n"
"  }\n"
"  </script>\n"
"  <br />\n"
"  <br />\n"
"  <label for=\"data_owner\"><b>Name</b> für Namensnennung in Lizenz (optional bei CC0)</label>\n"
"  <br />\n"
"  <input type=\"text\" name=\"data_owner\" id=\"owner\" maxlength=\"40\" placeholder=\"Name (optional bei CC0)\" value=\"{{DATAOWNER}}\">\n"
"  <br />\n"
"  <br />\n"
"  <input type=\"submit\">\n"
"</form>\n"
"<br />\n"
"<p>Hinweise: <br />\n"
"Die Eingabe neuer Daten überschreibt die alten Daten (z.B. bei Tippfehler oder Standortwechsel).\n"
"Alle von Ihnen eingegeben Informationen sind offen in der Datenbank abrufbar.\n"
"</p>\n"
"</body>\n"
"\n"
"</html";


///instance of FrostManager class and constructor
// frostManager(String esp32_chipId, String SDS_SerialNumber, <which temp sensor to use>, String Temp-sensor_SerialNumber, float coordinates[]={longitude, latitude, abovenn, aboveground}, String human-readable-location)
FrostManager *frostManager;
/// License Property Instance
LicenseProperty* mylicenseproperty;

//******************************************************** Variables ***************************

///OLED Font Created by http://oleddisplay.squix.ch/ Consider a donation
const unsigned  char Syncopate_Bold_9[] PROGMEM = {
  0x0E, // Width: 14
  0x0A, // Height: 10
  0x20, // First Char: 32
  0xE0, // Numbers of Chars: 224

  // Jump Table:
  0xFF, 0xFF, 0x00, 0x03,  // 32:65535
  0x00, 0x00, 0x05, 0x03,  // 33:0
  0x00, 0x05, 0x07, 0x04,  // 34:5
  0x00, 0x0C, 0x0B, 0x07,  // 35:12
  0x00, 0x17, 0x0D, 0x08,  // 36:23
  0x00, 0x24, 0x0F, 0x09,  // 37:36
  0x00, 0x33, 0x11, 0x09,  // 38:51
  0x00, 0x44, 0x03, 0x02,  // 39:68
  0x00, 0x47, 0x06, 0x04,  // 40:71
  0x00, 0x4D, 0x05, 0x04,  // 41:77
  0x00, 0x52, 0x07, 0x05,  // 42:82
  0x00, 0x59, 0x0B, 0x06,  // 43:89
  0x00, 0x64, 0x03, 0x02,  // 44:100
  0x00, 0x67, 0x07, 0x05,  // 45:103
  0x00, 0x6E, 0x03, 0x03,  // 46:110
  0x00, 0x71, 0x09, 0x05,  // 47:113
  0x00, 0x7A, 0x0D, 0x08,  // 48:122
  0x00, 0x87, 0x05, 0x04,  // 49:135
  0x00, 0x8C, 0x0B, 0x07,  // 50:140
  0x00, 0x97, 0x0B, 0x07,  // 51:151
  0x00, 0xA2, 0x0B, 0x07,  // 52:162
  0x00, 0xAD, 0x0B, 0x07,  // 53:173
  0x00, 0xB8, 0x0D, 0x08,  // 54:184
  0x00, 0xC5, 0x0B, 0x06,  // 55:197
  0x00, 0xD0, 0x0D, 0x08,  // 56:208
  0x00, 0xDD, 0x0D, 0x08,  // 57:221
  0x00, 0xEA, 0x03, 0x03,  // 58:234
  0x00, 0xED, 0x05, 0x03,  // 59:237
  0x00, 0xF2, 0x09, 0x06,  // 60:242
  0x00, 0xFB, 0x09, 0x06,  // 61:251
  0x01, 0x04, 0x09, 0x06,  // 62:260
  0x01, 0x0D, 0x09, 0x06,  // 63:269
  0x01, 0x16, 0x0F, 0x09,  // 64:278
  0x01, 0x25, 0x0F, 0x08,  // 65:293
  0x01, 0x34, 0x0D, 0x08,  // 66:308
  0x01, 0x41, 0x0D, 0x08,  // 67:321
  0x01, 0x4E, 0x0F, 0x09,  // 68:334
  0x01, 0x5D, 0x0F, 0x08,  // 69:349
  0x01, 0x6C, 0x0B, 0x07,  // 70:364
  0x01, 0x77, 0x0D, 0x08,  // 71:375
  0x01, 0x84, 0x0F, 0x09,  // 72:388
  0x01, 0x93, 0x03, 0x04,  // 73:403
  0x01, 0x96, 0x09, 0x06,  // 74:406
  0x01, 0x9F, 0x0F, 0x08,  // 75:415
  0x01, 0xAE, 0x0B, 0x07,  // 76:430
  0x01, 0xB9, 0x13, 0x0B,  // 77:441
  0x01, 0xCC, 0x0F, 0x09,  // 78:460
  0x01, 0xDB, 0x0F, 0x09,  // 79:475
  0x01, 0xEA, 0x0D, 0x08,  // 80:490
  0x01, 0xF7, 0x11, 0x09,  // 81:503
  0x02, 0x08, 0x0F, 0x08,  // 82:520
  0x02, 0x17, 0x0B, 0x07,  // 83:535
  0x02, 0x22, 0x0D, 0x07,  // 84:546
  0x02, 0x2F, 0x0D, 0x08,  // 85:559
  0x02, 0x3C, 0x0F, 0x08,  // 86:572
  0x02, 0x4B, 0x17, 0x0C,  // 87:587
  0x02, 0x62, 0x0F, 0x08,  // 88:610
  0x02, 0x71, 0x0D, 0x08,  // 89:625
  0x02, 0x7E, 0x0D, 0x08,  // 90:638
  0x02, 0x8B, 0x07, 0x04,  // 91:651
  0x02, 0x92, 0x09, 0x05,  // 92:658
  0x02, 0x9B, 0x05, 0x04,  // 93:667
  0x02, 0xA0, 0x09, 0x05,  // 94:672
  0x02, 0xA9, 0x0A, 0x05,  // 95:681
  0x02, 0xB3, 0x05, 0x05,  // 96:691
  0x02, 0xB8, 0x0D, 0x08,  // 97:696
  0x02, 0xC5, 0x0D, 0x08,  // 98:709
  0x02, 0xD2, 0x0D, 0x08,  // 99:722
  0x02, 0xDF, 0x0F, 0x09,  // 100:735
  0x02, 0xEE, 0x0F, 0x09,  // 101:750
  0x02, 0xFD, 0x0D, 0x08,  // 102:765
  0x03, 0x0A, 0x0D, 0x08,  // 103:778
  0x03, 0x17, 0x0F, 0x09,  // 104:791
  0x03, 0x26, 0x03, 0x04,  // 105:806
  0x03, 0x29, 0x09, 0x06,  // 106:809
  0x03, 0x32, 0x0F, 0x09,  // 107:818
  0x03, 0x41, 0x0B, 0x07,  // 108:833
  0x03, 0x4C, 0x13, 0x0B,  // 109:844
  0x03, 0x5F, 0x0F, 0x09,  // 110:863
  0x03, 0x6E, 0x0F, 0x09,  // 111:878
  0x03, 0x7D, 0x0D, 0x08,  // 112:893
  0x03, 0x8A, 0x11, 0x09,  // 113:906
  0x03, 0x9B, 0x0B, 0x07,  // 114:923
  0x03, 0xA6, 0x0B, 0x07,  // 115:934
  0x03, 0xB1, 0x0D, 0x07,  // 116:945
  0x03, 0xBE, 0x0F, 0x09,  // 117:958
  0x03, 0xCD, 0x0F, 0x08,  // 118:973
  0x03, 0xDC, 0x17, 0x0C,  // 119:988
  0x03, 0xF3, 0x0F, 0x08,  // 120:1011
  0x04, 0x02, 0x0D, 0x08,  // 121:1026
  0x04, 0x0F, 0x0D, 0x08,  // 122:1039
  0x04, 0x1C, 0x05, 0x04,  // 123:1052
  0x04, 0x21, 0x04, 0x03,  // 124:1057
  0x04, 0x25, 0x07, 0x04,  // 125:1061
  0x04, 0x2C, 0x09, 0x06,  // 126:1068
  0xFF, 0xFF, 0x00, 0x09,  // 127:65535
  0xFF, 0xFF, 0x00, 0x09,  // 128:65535
  0xFF, 0xFF, 0x00, 0x09,  // 129:65535
  0xFF, 0xFF, 0x00, 0x09,  // 130:65535
  0xFF, 0xFF, 0x00, 0x09,  // 131:65535
  0xFF, 0xFF, 0x00, 0x09,  // 132:65535
  0xFF, 0xFF, 0x00, 0x09,  // 133:65535
  0xFF, 0xFF, 0x00, 0x09,  // 134:65535
  0xFF, 0xFF, 0x00, 0x09,  // 135:65535
  0xFF, 0xFF, 0x00, 0x09,  // 136:65535
  0xFF, 0xFF, 0x00, 0x09,  // 137:65535
  0xFF, 0xFF, 0x00, 0x09,  // 138:65535
  0xFF, 0xFF, 0x00, 0x09,  // 139:65535
  0xFF, 0xFF, 0x00, 0x09,  // 140:65535
  0xFF, 0xFF, 0x00, 0x09,  // 141:65535
  0xFF, 0xFF, 0x00, 0x09,  // 142:65535
  0xFF, 0xFF, 0x00, 0x09,  // 143:65535
  0xFF, 0xFF, 0x00, 0x09,  // 144:65535
  0xFF, 0xFF, 0x00, 0x09,  // 145:65535
  0xFF, 0xFF, 0x00, 0x09,  // 146:65535
  0xFF, 0xFF, 0x00, 0x09,  // 147:65535
  0xFF, 0xFF, 0x00, 0x09,  // 148:65535
  0xFF, 0xFF, 0x00, 0x09,  // 149:65535
  0xFF, 0xFF, 0x00, 0x09,  // 150:65535
  0xFF, 0xFF, 0x00, 0x09,  // 151:65535
  0xFF, 0xFF, 0x00, 0x09,  // 152:65535
  0xFF, 0xFF, 0x00, 0x09,  // 153:65535
  0xFF, 0xFF, 0x00, 0x09,  // 154:65535
  0xFF, 0xFF, 0x00, 0x09,  // 155:65535
  0xFF, 0xFF, 0x00, 0x09,  // 156:65535
  0xFF, 0xFF, 0x00, 0x09,  // 157:65535
  0xFF, 0xFF, 0x00, 0x09,  // 158:65535
  0xFF, 0xFF, 0x00, 0x09,  // 159:65535
  0xFF, 0xFF, 0x00, 0x03,  // 160:65535
  0x04, 0x35, 0x05, 0x03,  // 161:1077
  0x04, 0x3A, 0x0B, 0x07,  // 162:1082
  0x04, 0x45, 0x0D, 0x08,  // 163:1093
  0x04, 0x52, 0x09, 0x05,  // 164:1106
  0x04, 0x5B, 0x0D, 0x08,  // 165:1115
  0x04, 0x68, 0x04, 0x03,  // 166:1128
  0x04, 0x6C, 0x07, 0x05,  // 167:1132
  0x04, 0x73, 0x07, 0x05,  // 168:1139
  0x04, 0x7A, 0x0D, 0x07,  // 169:1146
  0x04, 0x87, 0x07, 0x05,  // 170:1159
  0x04, 0x8E, 0x09, 0x05,  // 171:1166
  0x04, 0x97, 0x09, 0x06,  // 172:1175
  0x04, 0xA0, 0x07, 0x05,  // 173:1184
  0x04, 0xA7, 0x09, 0x05,  // 174:1191
  0x04, 0xB0, 0x07, 0x05,  // 175:1200
  0x04, 0xB7, 0x05, 0x04,  // 176:1207
  0x04, 0xBC, 0x0B, 0x06,  // 177:1212
  0x04, 0xC7, 0x07, 0x05,  // 178:1223
  0x04, 0xCE, 0x07, 0x05,  // 179:1230
  0x04, 0xD5, 0x07, 0x05,  // 180:1237
  0x04, 0xDC, 0x0B, 0x07,  // 181:1244
  0x04, 0xE7, 0x09, 0x06,  // 182:1255
  0x04, 0xF0, 0x03, 0x03,  // 183:1264
  0x04, 0xF3, 0x08, 0x05,  // 184:1267
  0x04, 0xFB, 0x03, 0x03,  // 185:1275
  0x04, 0xFE, 0x09, 0x06,  // 186:1278
  0x05, 0x07, 0x09, 0x05,  // 187:1287
  0x05, 0x10, 0x11, 0x09,  // 188:1296
  0x05, 0x21, 0x0F, 0x09,  // 189:1313
  0x05, 0x30, 0x13, 0x0A,  // 190:1328
  0x05, 0x43, 0x09, 0x05,  // 191:1347
  0x05, 0x4C, 0x0F, 0x08,  // 192:1356
  0x05, 0x5B, 0x0F, 0x08,  // 193:1371
  0x05, 0x6A, 0x0F, 0x08,  // 194:1386
  0x05, 0x79, 0x0F, 0x08,  // 195:1401
  0x05, 0x88, 0x0F, 0x08,  // 196:1416
  0x05, 0x97, 0x0F, 0x08,  // 197:1431
  0x05, 0xA6, 0x15, 0x0C,  // 198:1446
  0x05, 0xBB, 0x0D, 0x08,  // 199:1467
  0x05, 0xC8, 0x0F, 0x08,  // 200:1480
  0x05, 0xD7, 0x0F, 0x08,  // 201:1495
  0x05, 0xE6, 0x0F, 0x08,  // 202:1510
  0x05, 0xF5, 0x0F, 0x08,  // 203:1525
  0x06, 0x04, 0x03, 0x04,  // 204:1540
  0x06, 0x07, 0x03, 0x04,  // 205:1543
  0x06, 0x0A, 0x05, 0x04,  // 206:1546
  0x06, 0x0F, 0x05, 0x04,  // 207:1551
  0x06, 0x14, 0x0F, 0x09,  // 208:1556
  0x06, 0x23, 0x0F, 0x09,  // 209:1571
  0x06, 0x32, 0x0F, 0x09,  // 210:1586
  0x06, 0x41, 0x0F, 0x09,  // 211:1601
  0x06, 0x50, 0x0F, 0x09,  // 212:1616
  0x06, 0x5F, 0x0F, 0x09,  // 213:1631
  0x06, 0x6E, 0x0F, 0x09,  // 214:1646
  0x06, 0x7D, 0x09, 0x06,  // 215:1661
  0x06, 0x86, 0x0F, 0x09,  // 216:1670
  0x06, 0x95, 0x0D, 0x09,  // 217:1685
  0x06, 0xA2, 0x0D, 0x09,  // 218:1698
  0x06, 0xAF, 0x0D, 0x09,  // 219:1711
  0x06, 0xBC, 0x0D, 0x09,  // 220:1724
  0x06, 0xC9, 0x0D, 0x08,  // 221:1737
  0x06, 0xD6, 0x0D, 0x08,  // 222:1750
  0x06, 0xE3, 0x0F, 0x09,  // 223:1763
  0x06, 0xF2, 0x0D, 0x09,  // 224:1778
  0x06, 0xFF, 0x0D, 0x09,  // 225:1791
  0x07, 0x0C, 0x0D, 0x09,  // 226:1804
  0x07, 0x19, 0x0D, 0x09,  // 227:1817
  0x07, 0x26, 0x0D, 0x09,  // 228:1830
  0x07, 0x33, 0x0D, 0x09,  // 229:1843
  0x07, 0x40, 0x1B, 0x0E,  // 230:1856
  0x07, 0x5B, 0x0D, 0x08,  // 231:1883
  0x07, 0x68, 0x0F, 0x09,  // 232:1896
  0x07, 0x77, 0x0F, 0x09,  // 233:1911
  0x07, 0x86, 0x0F, 0x09,  // 234:1926
  0x07, 0x95, 0x0F, 0x09,  // 235:1941
  0x07, 0xA4, 0x03, 0x04,  // 236:1956
  0x07, 0xA7, 0x03, 0x04,  // 237:1959
  0x07, 0xAA, 0x05, 0x04,  // 238:1962
  0x07, 0xAF, 0x05, 0x04,  // 239:1967
  0x07, 0xB4, 0x0F, 0x09,  // 240:1972
  0x07, 0xC3, 0x0F, 0x09,  // 241:1987
  0x07, 0xD2, 0x0F, 0x09,  // 242:2002
  0x07, 0xE1, 0x0F, 0x09,  // 243:2017
  0x07, 0xF0, 0x0F, 0x09,  // 244:2032
  0x07, 0xFF, 0x0F, 0x09,  // 245:2047
  0x08, 0x0E, 0x0F, 0x09,  // 246:2062
  0x08, 0x1D, 0x09, 0x06,  // 247:2077
  0x08, 0x26, 0x0F, 0x09,  // 248:2086
  0x08, 0x35, 0x0F, 0x09,  // 249:2101
  0x08, 0x44, 0x0F, 0x09,  // 250:2116
  0x08, 0x53, 0x0F, 0x09,  // 251:2131
  0x08, 0x62, 0x0F, 0x09,  // 252:2146
  0x08, 0x71, 0x0D, 0x09,  // 253:2161
  0x08, 0x7E, 0x0D, 0x08,  // 254:2174
  0x08, 0x8B, 0x0D, 0x09,  // 255:2187

  // Font Data:
  0x00,0x00,0xBC,0x00,0x0C, // 33
  0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C, // 34
  0x40,0x00,0xC8,0x00,0x7C,0x00,0xCC,0x00,0xFC,0x00,0x4C, // 35
  0x00,0x00,0xD8,0x00,0x94,0x00,0xFE,0x01,0xB4,0x00,0xA4,0x00,0x64, // 36
  0x1C,0x00,0x94,0x00,0x54,0x00,0x28,0x00,0x10,0x00,0x48,0x00,0xA4,0x00,0xE0, // 37
  0x00,0x00,0x7C,0x00,0xB4,0x00,0xB4,0x00,0xF4,0x00,0xCC,0x00,0xE0,0x00,0xA0,0x00,0x80, // 38
  0x0C,0x00,0x0C, // 39
  0x00,0x00,0xFC,0x00,0x86,0x01,  // 40
  0x00,0x00,0x86,0x01,0xFC, // 41
  0x28,0x00,0x18,0x00,0x3C,0x00,0x18, // 42
  0x00,0x00,0x10,0x00,0x10,0x00,0x78,0x00,0x10,0x00,0x10, // 43
  0xC0,0x01,0xC0, // 44
  0x00,0x00,0x10,0x00,0x10,0x00,0x10, // 45
  0x00,0x00,0xC0, // 46
  0x80,0x00,0xC0,0x00,0x70,0x00,0x1C,0x00,0x04, // 47
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x78, // 48
  0x0C,0x00,0x04,0x00,0xFC, // 49
  0x00,0x00,0xCC,0x00,0xC4,0x00,0xE4,0x00,0xE4,0x00,0xBC, // 50
  0x00,0x00,0xCC,0x00,0x84,0x00,0x94,0x00,0x94,0x00,0xEC, // 51
  0x60,0x00,0x70,0x00,0x58,0x00,0x4C,0x00,0xFC,0x00,0x40, // 52
  0x00,0x00,0xDC,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0xE4, // 53
  0x00,0x00,0x78,0x00,0xD4,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0x64, // 54
  0x04,0x00,0x84,0x00,0xC4,0x00,0x74,0x00,0x3C,0x00,0x0C, // 55
  0x00,0x00,0x78,0x00,0xB4,0x00,0x94,0x00,0x94,0x00,0xB4,0x00,0xF8, // 56
  0x98,0x00,0xA4,0x00,0xA4,0x00,0xA4,0x00,0xA4,0x00,0xAC,0x00,0x78, // 57
  0x00,0x00,0xD8, // 58
  0x00,0x00,0x98,0x01,0x80, // 59
  0x00,0x00,0x10,0x00,0x28,0x00,0x28,0x00,0x6C, // 60
  0x00,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x48, // 61
  0x00,0x00,0x6C,0x00,0x28,0x00,0x38,0x00,0x10, // 62
  0x00,0x00,0xA4,0x00,0x24,0x00,0x24,0x00,0x1C, // 63
  0x00,0x00,0x70,0x00,0x88,0x00,0xF4,0x00,0xCC,0x00,0xFC,0x00,0x44,0x00,0x38, // 64
  0x80,0x00,0xE0,0x00,0x78,0x00,0x5C,0x00,0x5C,0x00,0x78,0x00,0xE0,0x00,0x80, // 65
  0x00,0x00,0xFC,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0xEC, // 66
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0xCC, // 67
  0x00,0x00,0xFC,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x78, // 68
  0x00,0x00,0xFC,0x00,0xFC,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0x80, // 69
  0x00,0x00,0xFC,0x00,0x14,0x00,0x14,0x00,0x14,0x00,0x14, // 70
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0xA4,0x00,0xA4,0x00,0xEC, // 71
  0x00,0x00,0xFC,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0xFC, // 72
  0x00,0x00,0xFC, // 73
  0xC0,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x7C, // 74
  0x00,0x00,0xFC,0x00,0x30,0x00,0x78,0x00,0x48,0x00,0xC4,0x00,0x84,0x00,0x84, // 75
  0x00,0x00,0xFC,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80, // 76
  0x00,0x00,0xFC,0x00,0x1C,0x00,0x38,0x00,0xE0,0x00,0x80,0x00,0xE0,0x00,0x38,0x00,0x1C,0x00,0xFC, // 77
  0x00,0x00,0xFC,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,0x60,0x00,0xC0,0x00,0xFC, // 78
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x78, // 79
  0x00,0x00,0xFC,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x3C, // 80
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0xC4,0x00,0xE4,0x00,0xC4,0x00,0xF8,0x00,0x80, // 81
  0x00,0x00,0xFC,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x64,0x00,0xFC,0x00,0x80, // 82
  0x00,0x00,0xD8,0x00,0x94,0x00,0xB4,0x00,0xB4,0x00,0xEC, // 83
  0x04,0x00,0x04,0x00,0x04,0x00,0xFC,0x00,0x04,0x00,0x04,0x00,0x04, // 84
  0x00,0x00,0x7C,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x7C, // 85
  0x04,0x00,0x1C,0x00,0x78,0x00,0xE0,0x00,0xC0,0x00,0x70,0x00,0x1C,0x00,0x04, // 86
  0x04,0x00,0x1C,0x00,0xF0,0x00,0xC0,0x00,0xF8,0x00,0x1C,0x00,0x1C,0x00,0xF0,0x00,0xC0,0x00,0x78,0x00,0x1C,0x00,0x04, // 87
  0x84,0x00,0x84,0x00,0xEC,0x00,0x38,0x00,0x78,0x00,0xCC,0x00,0x84,0x00,0x84, // 88
  0x04,0x00,0x0C,0x00,0x18,0x00,0xF0,0x00,0x18,0x00,0x0C,0x00,0x04, // 89
  0x00,0x00,0xC4,0x00,0xE4,0x00,0xB4,0x00,0x9C,0x00,0x8C,0x00,0x8C, // 90
  0x00,0x00,0xFC,0x00,0x84,0x00,0x84, // 91
  0x04,0x00,0x0C,0x00,0x38,0x00,0xE0,0x00,0x80, // 92
  0x84,0x00,0x84,0x00,0xFC, // 93
  0x10,0x00,0x18,0x00,0x0C,0x00,0x1C,0x00,0x10, // 94
  0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,0x02,  // 95
  0x00,0x00,0x02,0x00,0x04, // 96
  0x00,0x00,0x6C,0x00,0xA4,0x00,0x94,0x00,0x94,0x00,0xD4,0x00,0xF8, // 97
  0x00,0x00,0xFC,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0xEC, // 98
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0xCC, // 99
  0x00,0x00,0xFC,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x78, // 100
  0x00,0x00,0x78,0x00,0xEC,0x00,0xA4,0x00,0xA4,0x00,0xA4,0x00,0xBC,0x00,0xF8, // 101
  0x00,0x00,0xFC,0x00,0x14,0x00,0x14,0x00,0x14,0x00,0x14,0x00,0x14, // 102
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0xA4,0x00,0xA4,0x00,0xEC, // 103
  0x00,0x00,0xFC,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0xFC, // 104
  0x00,0x00,0xFC, // 105
  0xC0,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x7C, // 106
  0x00,0x00,0xFC,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x28,0x00,0xEC,0x00,0x80, // 107
  0x00,0x00,0xFC,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80, // 108
  0x00,0x00,0xFC,0x00,0x08,0x00,0x04,0x00,0x04,0x00,0xFC,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0xF8, // 109
  0x00,0x00,0xFC,0x00,0x08,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,0xF8, // 110
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x78, // 111
  0x00,0x00,0xFC,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x24,0x00,0x3C, // 112
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0xF8,0x00,0x80, // 113
  0x00,0x00,0xFC,0x00,0x08,0x00,0x04,0x00,0x04,0x00,0x04, // 114
  0x00,0x00,0xD8,0x00,0x94,0x00,0xB4,0x00,0xB4,0x00,0xEC, // 115
  0x04,0x00,0x04,0x00,0x04,0x00,0xFC,0x00,0x04,0x00,0x04,0x00,0x04, // 116
  0x00,0x00,0x7C,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x40,0x00,0xFC, // 117
  0x04,0x00,0x1C,0x00,0x78,0x00,0xE0,0x00,0xC0,0x00,0x70,0x00,0x1C,0x00,0x04, // 118
  0x04,0x00,0x1C,0x00,0xF0,0x00,0xC0,0x00,0xF8,0x00,0x1C,0x00,0x1C,0x00,0xF0,0x00,0xC0,0x00,0x78,0x00,0x1C,0x00,0x04, // 119
  0x84,0x00,0x84,0x00,0xEC,0x00,0x38,0x00,0x78,0x00,0xCC,0x00,0x84,0x00,0x84, // 120
  0x00,0x00,0xDC,0x00,0xA0,0x00,0xA0,0x00,0xA0,0x00,0xB0,0x00,0x7C, // 121
  0x00,0x00,0xC4,0x00,0xE4,0x00,0xB4,0x00,0x9C,0x00,0x8C,0x00,0x8C, // 122
  0x10,0x00,0xEC,0x00,0x84, // 123
  0x00,0x00,0xFE,0x03,  // 124
  0x00,0x00,0x84,0x00,0xEC,0x00,0x10, // 125
  0x00,0x00,0x30,0x00,0x10,0x00,0x20,0x00,0x30, // 126
  0x00,0x00,0xF4,0x00,0xC0, // 161
  0x78,0x00,0xCC,0x00,0xCC,0x00,0xFE,0x01,0x84,0x00,0xCC, // 162
  0x00,0x00,0x90,0x00,0xF8,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0x84, // 163
  0x00,0x00,0x78,0x00,0x48,0x00,0x78,0x00,0x48, // 164
  0x04,0x00,0x54,0x00,0x58,0x00,0xF0,0x00,0x58,0x00,0x54,0x00,0x04, // 165
  0x00,0x00,0x9C,0x03,  // 166
  0x00,0x00,0xBC,0x00,0xB4,0x00,0xF4, // 167
  0x00,0x00,0x04,0x00,0x00,0x00,0x04, // 168
  0x78,0x00,0x48,0x00,0xB4,0x00,0xCC,0x00,0xCC,0x00,0x48,0x00,0x78, // 169
  0x3C,0x00,0x3C,0x00,0x3C,0x00,0x3C, // 170
  0x20,0x00,0x78,0x00,0x50,0x00,0x30,0x00,0x48, // 171
  0x00,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x70, // 172
  0x00,0x00,0x10,0x00,0x10,0x00,0x10, // 173
  0x18,0x00,0x24,0x00,0x3C,0x00,0x3C,0x00,0x18, // 174
  0x00,0x00,0x04,0x00,0x04,0x00,0x04, // 175
  0x1C,0x00,0x14,0x00,0x1C, // 176
  0x00,0x00,0x90,0x00,0x90,0x00,0xBC,0x00,0x90,0x00,0x10, // 177
  0x24,0x00,0x34,0x00,0x34,0x00,0x2C, // 178
  0x24,0x00,0x24,0x00,0x2C,0x00,0x3C, // 179
  0x00,0x00,0x04,0x00,0x04,0x00,0x02, // 180
  0x00,0x02,0xF0,0x03,0xB0,0x00,0x80,0x00,0xC0,0x00,0xF0, // 181
  0x00,0x00,0x1C,0x00,0x1C,0x00,0xFC,0x00,0xFC, // 182
  0x00,0x00,0x30, // 183
  0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x03,  // 184
  0x04,0x00,0x3C, // 185
  0x28,0x00,0x34,0x00,0x34,0x00,0x34,0x00,0x1C, // 186
  0x50,0x00,0x78,0x00,0x20,0x00,0x78,0x00,0x30, // 187
  0x04,0x00,0x9C,0x00,0x40,0x00,0x20,0x00,0x10,0x00,0xC8,0x00,0xA4,0x00,0xE0,0x00,0x80, // 188
  0x04,0x00,0x9C,0x00,0x40,0x00,0x30,0x00,0x18,0x00,0xA4,0x00,0xA0,0x00,0xE0, // 189
  0x00,0x00,0x14,0x00,0x9C,0x00,0x94,0x00,0x60,0x00,0x30,0x00,0xC8,0x00,0xA4,0x00,0xE0,0x00,0x80, // 190
  0x00,0x00,0xE0,0x00,0xA0,0x00,0x94,0x00,0x80, // 191
  0x80,0x00,0xE0,0x00,0x78,0x00,0x5C,0x00,0x5D,0x00,0x78,0x00,0xE0,0x00,0x80, // 192
  0x80,0x00,0xE0,0x00,0x78,0x00,0x5D,0x00,0x5D,0x00,0x78,0x00,0xE0,0x00,0x80, // 193
  0x80,0x00,0xE0,0x00,0x79,0x00,0x5D,0x00,0x5C,0x00,0x79,0x00,0xE0,0x00,0x80, // 194
  0x80,0x00,0xE0,0x00,0x79,0x00,0x5C,0x00,0x5D,0x00,0x79,0x00,0xE0,0x00,0x80, // 195
  0x80,0x00,0xE0,0x00,0x78,0x00,0x5D,0x00,0x5C,0x00,0x79,0x00,0xE0,0x00,0x80, // 196
  0x80,0x00,0xE0,0x00,0x78,0x00,0x5D,0x00,0x5D,0x00,0x79,0x00,0xE0,0x00,0x80, // 197
  0x80,0x00,0xE0,0x00,0x70,0x00,0x5C,0x00,0x44,0x00,0xFC,0x00,0xFC,0x00,0xA4,0x00,0xA4,0x00,0xA4,0x00,0xA4, // 198
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x03,0x84,0x03,0xCC, // 199
  0x00,0x00,0xFC,0x00,0xFC,0x00,0x94,0x00,0x95,0x00,0x94,0x00,0x94,0x00,0x80, // 200
  0x00,0x00,0xFC,0x00,0xFC,0x00,0x95,0x00,0x95,0x00,0x94,0x00,0x94,0x00,0x80, // 201
  0x00,0x00,0xFC,0x00,0xFD,0x00,0x95,0x00,0x94,0x00,0x95,0x00,0x94,0x00,0x80, // 202
  0x00,0x00,0xFC,0x00,0xFC,0x00,0x95,0x00,0x94,0x00,0x95,0x00,0x94,0x00,0x80, // 203
  0x00,0x00,0xFD, // 204
  0x01,0x00,0xFD, // 205
  0x01,0x00,0xFC,0x00,0x01, // 206
  0x01,0x00,0xFC,0x00,0x01, // 207
  0x10,0x00,0xFC,0x00,0x94,0x00,0x94,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x78, // 208
  0x00,0x00,0xFC,0x00,0x0D,0x00,0x18,0x00,0x31,0x00,0x61,0x00,0xC0,0x00,0xFC, // 209
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x84,0x00,0x78, // 210
  0x00,0x00,0x78,0x00,0x84,0x00,0x85,0x00,0x85,0x00,0x84,0x00,0x84,0x00,0x78, // 211
  0x00,0x00,0x78,0x00,0x85,0x00,0x85,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x78, // 212
  0x00,0x00,0x78,0x00,0x85,0x00,0x84,0x00,0x85,0x00,0x85,0x00,0x84,0x00,0x78, // 213
  0x00,0x00,0x78,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x78, // 214
  0x00,0x00,0x48,0x00,0x30,0x00,0x30,0x00,0x78, // 215
  0x00,0x00,0x78,0x01,0xC4,0x00,0xE4,0x00,0xB4,0x00,0x9C,0x00,0x8C,0x00,0x7A, // 216
  0x00,0x00,0x7C,0x00,0x80,0x00,0x80,0x00,0x81,0x00,0x80,0x00,0x7C, // 217
  0x00,0x00,0x7C,0x00,0x80,0x00,0x81,0x00,0x81,0x00,0x80,0x00,0x7C, // 218
  0x00,0x00,0x7C,0x00,0x81,0x00,0x81,0x00,0x80,0x00,0x81,0x00,0x7C, // 219
  0x00,0x00,0x7C,0x00,0x80,0x00,0x81,0x00,0x80,0x00,0x81,0x00,0x7C, // 220
  0x04,0x00,0x0C,0x00,0x18,0x00,0xF1,0x00,0x19,0x00,0x0C,0x00,0x04, // 221
  0x00,0x00,0xFC,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x78, // 222
  0x00,0x00,0xF8,0x00,0x04,0x00,0x04,0x00,0x84,0x00,0x94,0x00,0x94,0x00,0xF8, // 223
  0x00,0x00,0x6C,0x00,0xA4,0x00,0x94,0x00,0x95,0x00,0xD4,0x00,0xF8, // 224
  0x00,0x00,0x6C,0x00,0xA4,0x00,0x95,0x00,0x95,0x00,0xD4,0x00,0xF8, // 225
  0x00,0x00,0x6C,0x00,0xA5,0x00,0x95,0x00,0x94,0x00,0xD5,0x00,0xF8, // 226
  0x00,0x00,0x6C,0x00,0xA5,0x00,0x94,0x00,0x95,0x00,0xD5,0x00,0xF8, // 227
  0x00,0x00,0x6C,0x00,0xA4,0x00,0x95,0x00,0x94,0x00,0xD5,0x00,0xF8, // 228
  0x00,0x00,0x6C,0x00,0xA4,0x00,0x95,0x00,0x95,0x00,0xD5,0x00,0xF8, // 229
  0x00,0x00,0xEC,0x00,0x94,0x00,0x94,0x00,0x94,0x00,0xD4,0x00,0x7C,0x00,0x78,0x00,0xAC,0x00,0xA4,0x00,0xA4,0x00,0xAC,0x00,0xB8,0x00,0x30, // 230
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x84,0x03,0x84,0x03,0xCC, // 231
  0x00,0x00,0x78,0x00,0xEC,0x00,0xA4,0x00,0xA5,0x00,0xA4,0x00,0xBC,0x00,0xF8, // 232
  0x00,0x00,0x78,0x00,0xEC,0x00,0xA5,0x00,0xA5,0x00,0xA4,0x00,0xBC,0x00,0xF8, // 233
  0x00,0x00,0x78,0x00,0xED,0x00,0xA5,0x00,0xA4,0x00,0xA5,0x00,0xBC,0x00,0xF8, // 234
  0x00,0x00,0x78,0x00,0xEC,0x00,0xA5,0x00,0xA4,0x00,0xA5,0x00,0xBC,0x00,0xF8, // 235
  0x00,0x00,0xFD, // 236
  0x01,0x00,0xFD, // 237
  0x01,0x00,0xFC,0x00,0x01, // 238
  0x01,0x00,0xFC,0x00,0x01, // 239
  0x10,0x00,0xFC,0x00,0x94,0x00,0x94,0x00,0x84,0x00,0x84,0x00,0x84,0x00,0x78, // 240
  0x00,0x00,0xFC,0x00,0x08,0x00,0x05,0x00,0x04,0x00,0x05,0x00,0x05,0x00,0xF8, // 241
  0x00,0x00,0x78,0x00,0x84,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x84,0x00,0x78, // 242
  0x00,0x00,0x78,0x00,0x84,0x00,0x85,0x00,0x85,0x00,0x84,0x00,0x84,0x00,0x78, // 243
  0x00,0x00,0x78,0x00,0x85,0x00,0x85,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x78, // 244
  0x00,0x00,0x78,0x00,0x85,0x00,0x84,0x00,0x85,0x00,0x85,0x00,0x84,0x00,0x78, // 245
  0x00,0x00,0x78,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x85,0x00,0x84,0x00,0x78, // 246
  0x00,0x00,0x10,0x00,0x10,0x00,0x58,0x00,0x10, // 247
  0x00,0x00,0x78,0x01,0xC4,0x00,0xE4,0x00,0xB4,0x00,0x9C,0x00,0x8C,0x00,0x7A, // 248
  0x00,0x00,0x7C,0x00,0x80,0x00,0x80,0x00,0x81,0x00,0x80,0x00,0x40,0x00,0xFC, // 249
  0x00,0x00,0x7C,0x00,0x80,0x00,0x81,0x00,0x81,0x00,0x80,0x00,0x40,0x00,0xFC, // 250
  0x00,0x00,0x7C,0x00,0x81,0x00,0x81,0x00,0x80,0x00,0x81,0x00,0x40,0x00,0xFC, // 251
  0x00,0x00,0x7C,0x00,0x80,0x00,0x81,0x00,0x80,0x00,0x81,0x00,0x40,0x00,0xFC, // 252
  0x00,0x00,0xDC,0x00,0xA0,0x00,0xA1,0x00,0xA1,0x00,0xB0,0x00,0x7C, // 253
  0x00,0x00,0xFC,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x48,0x00,0x78, // 254
  0x00,0x00,0xDC,0x00,0xA0,0x00,0xA1,0x00,0xA0,0x00,0xB1,0x00,0x7C  // 255
};
//******************************************************** Funtion Prototypes ***************************

// dynamic strings and getter for html content
//String viewCredential(PageArgument&);
//String changeCfg(PageArgument&);
//String getAddress(PageArgument&);
//String getStreet(PageArgument&);
//String getStreetnumber(PageArgument&);
//String getCity(PageArgument&);
//String getCountry(PageArgument&);
//String getLongitude(PageArgument&);
//String getLatitude(PageArgument&);
//String getHeightNN(PageArgument&);
//String getHeightGround(PageArgument&);
//String getRhtSensor(PageArgument&);
//String getSds011Id(PageArgument&);
//String getDataOwner(PageArgument&);
//String getDataUrlCC0(PageArgument&);
//String getDataUrlCCBY(PageArgument&);
//String getFirmwareVersion(PageArgument&);
//String getTimeTilReboot(PageArgument&);
//String getApMode(PageArgument&);
//String getWifiConnectivity(PageArgument&);


//******************************************************** FUNCTIONS ***************************

/// Custom methods to retrieve stored data (address) for insertion into HTML page
String getAddress(PageArgument& args) {
  return String(get_value("user_data", "address"));
}
/// Custom methods to retrieve stored data (longitude) for insertion into HTML page
String getLongitude(PageArgument& args) {
  return String(get_value("user_data", "longitude"));
}
/// Custom methods to retrieve stored data (latitude) for insertion into HTML page
String getLatitude(PageArgument& args) {
  return String(get_value("user_data", "latitude"));
}
/// Custom methods to retrieve stored data (data_owner) for insertion into HTML page
String getDataOwner(PageArgument& args) {
  return String(get_value("user_data", "data_owner"));
}
/// Custom methods to retrieve stored data (data_url_cc0) for insertion into HTML page
String getDataUrlCC0(PageArgument& args) {
  return data_url_cc0;
}
/// Custom methods to retrieve stored data (data_url_ccby) for insertion into HTML page
String getDataUrlCCBY(PageArgument& args) {
  return data_url_ccby;
}
/// Custom methods to retrieve stored data (FIRMWARE_VERSION) for insertion into HTML page
String getFirmwareVersion(PageArgument& args) {
  return F(FIRMWARE_VERSION);
}
/// Custom methods to retrieve stored data (rht_sensor) for insertion into HTML page
String getRhtSensor(PageArgument& args) {
  String tmp = String(get_value("user_data", "rht_sensor"));
  if (tmp == "") {
    return "keiner";
  }
  else {
    return tmp;
  }
}
/// Custom methods to retrieve stored data (sds011id) for insertion into HTML page
String getSds011Id(PageArgument& args) {
  return String(get_value("user_data", "sds011id"));
}
/// Custom methods to retrieve stored data (WifiConnectivity) for insertion into HTML page
String getWifiConnectivity(PageArgument& args) {
  Serial.println("Wifi-status: ");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("true");
    return (String)"true";
  }
  else {
    Serial.println("false");
    return (String)"false";
  }
}

/// Retrieve the credential entries from EEPROM, Build the SSID line with the lo tag.
String viewCredential(PageArgument& args) {
  AutoConnectCredential  ac(CREDENTIAL_OFFSET);
  struct station_config  entry;
  String content = "";
  uint8_t  count = ac.entries();          // Get number of entries.

  for (int8_t i = 0; i < count; i++) {    // Loads all entries.
    ac.load(i, &entry);
    // Build a SSID line of an HTML.
    content += String("<li>") + String((char *)entry.ssid) + String("</li>");
  }

  // Returns the '<li>SSID</li>' container.
  return content;
}
               
/// save formular data in eeprom
String changeCfg(PageArgument& args) {
  AutoConnectCredential  ac(CREDENTIAL_OFFSET);

  if (args.hasArg("address") && (args.arg("address").length() > 0))
  {
    set_value("user_data", "address", Server.arg("address").c_str());
  }

  if (args.hasArg("latitude") && (args.arg("latitude").length() > 0))
  {
    set_value("user_data", "latitude", Server.arg("latitude").c_str());
  }

  if (args.hasArg("longitude") && (args.arg("longitude").length() > 0))
  {
    set_value("user_data", "longitude", Server.arg("longitude").c_str());
  }

  if (args.hasArg("rht_sensor"))
  {
    set_value("user_data", "rht_sensor", Server.arg("rht_sensor").c_str());
  }

  if (args.hasArg("sds011id"))
  {
    set_value("user_data", "sds011id", Server.arg("sds011id").c_str());
  }

  if (args.hasArg("data_license"))
  {
    String cur_license = Server.arg("data_license").c_str();

    set_value("user_data", "data_license", Server.arg("data_license").c_str());

    if (cur_license == "CC0 1.0") { 
      set_value("user_data", "data_url", data_url_cc0);
    } 
    else {
      set_value("user_data", "data_url", data_url_ccby);
    } 
  }

  if (args.hasArg("data_owner"))
  {
    set_value("user_data", "data_owner", Server.arg("data_owner").c_str());
  }
  /*set_value("user_data", "address", "");
  set_value("user_data", "latitude", "");
  set_value("user_data", "longitude", "");
  set_value("user_data", "sds011id", "");
  set_value("user_data", "data_owner", "");*/

  // Returns the redirect response. The page is reloaded and its contents
  // are updated to the state after deletion. It returns 302 response
  // from inside this token handler.
  Server.sendHeader("Location", String("http://") + Server.client().localIP().toString() + String("/"));
  Server.send(302, "text/plain", "");
  Server.client().flush();
  Server.client().stop();

  // RESTART ESP TODO: test this!!!
  delay(2000);
  Serial.println("Rebooting...\n");
  delay(1000);
  ESP.restart();

  // Cancel automatic submission by PageBuilder.
  //  delPage.cancel();

  return "";
}

/// Get Standard time
String getIsoTime() {
  return (UTC.dateTime("Y-m-d\\TH:i:s") + ".000Z");
}
/// Get hours from the time
String getHourMinute() {
  return (UTC.dateTime("Hi"));
}
/// get chip id from MAC
String getChipId() 
{
  // save mac address and convert to char array.
  // only last 3 bytes of mac address are used, because the first 3 bytes are fixed and always the same on every board

  // get mac address
  // wasting memory for conversion last 3 bytes of byte mac into string mac. should be done better
  esp_efuse_mac_get_default(mac);

  char mac_str[18];
  // convert to string
  snprintf(mac_str, sizeof(mac_str), "%02x%02x%02x", mac[3], mac[4], mac[5]);
  unsigned long mac_decimal =  strtoul(mac_str, NULL, 16);
  esp32_chipid = String(mac_decimal);
  return esp32_chipid;
}

/// AutoConnect Menu Object
static const char PROGMEM autoconnectMenu[] = { AUTOCONNECT_LINK(BAR_24) };
/// page builder elements
// URL path as '/'
PageElement elmList(html,
{ { "SSID", viewCredential },
  { "ADDRESS", getAddress },
  //  { "STREET", getStreet },
  //  { "STREETNUMBER", getStreetnumber },
  //  { "CITY", getCity },
  //  { "COUNTRY", getCountry },
  { "LON", getLongitude },
  { "LAT", getLatitude },
  { "RHTSENSOR", getRhtSensor },
  { "SDS011ID", getSds011Id },
  { "DATAOWNER", getDataOwner },
  { "DATAURLCC0", getDataUrlCC0},
  { "DATAURLCCBY", getDataUrlCCBY},
  //  { "RESTTIME", getTimeTilReboot },
  //  { "APMODE", getApMode },
  { "FWVERSION", getFirmwareVersion},
  { "WIFICONNECTIVITY", getWifiConnectivity},
  { "AUTOCONNECT_MENU", [](PageArgument & args) {
      return String(FPSTR(autoconnectMenu));
    }
  }
});

/// Pagebuilder for root
PageBuilder rootPage("/", { elmList });
/// Pagebuilder for delete
PageElement elmCfg("{{DEL}}", {{ "DEL", changeCfg }});
/// Pagebuilder for change in the contents
PageBuilder cfgPage("/config", { elmCfg });























String  cfgTemp;

// -----------------------------------------------------------------------------
ACElement(cfgScript,  
"  <script src=\"http://www.openlayers.org/api/OpenLayers.js\"></script>\n"
"  <script>\n"
"    //global vars\n"
"    var latitude;\n"
"    var longitude;\n"
"    //var elevation;\n"
"    var address            = \"\";\n"
"    var wgsProjection      = new OpenLayers.Projection(\"EPSG:4326\");   // WGS 1984 Projection\n"
"    var mercatorProjection = new OpenLayers.Projection(\"EPSG:900913\"); // Spherical Mercator Projection\n"
"    var initialposition    = new OpenLayers.LonLat(8.424171,49.013034).transform(wgsProjection, mercatorProjection);\n"
"    var initialzoom        = 3;\n"
"    var position           = initialposition;\n"
"    var zoom               = 18;\n"
"    var mapnik             = new OpenLayers.Layer.OSM();\n"
"    var markers            = new OpenLayers.Layer.Markers(\"Markers\");\n"
"    var marker             = new OpenLayers.Marker(position);\n"
"    var touchNavigation    = new OpenLayers.Control.TouchNavigation({\n"
"                                 defaultClickOptions:{\n"
"                                     pixelTolerance: 10\n"
"                                 }\n"
"                             });\n"
"\n  "
"    // function to geocode: look up coordinates from address text field, populate lon and lat fields and update map \n"
"    function geolookup() {\n"
"      address = document.getElementById('inputAddress').value;\n"
"      var request = new XMLHttpRequest();\n"
"      var url = \"https://nominatim.openstreetmap.org/search/'\" + encodeURI(address) + \"'?format=json&limit=1\";\n"
"      request.open('GET', url, true);\n"
"      request.onload = function() {\n"
"        if (this.status >= 200 && this.status < 300) {\n"
"          var response = JSON.parse(this.responseText);\n"
"          if (response[0]) {\n"
"            latitude = Number(response[0].lat).toFixed(6);\n"
"            longitude = Number(response[0].lon).toFixed(6);\n"
"            document.getElementById(\"inputLon\").value = longitude;\n"
"            document.getElementById(\"inputLat\").value = latitude;\n"
"            updateMarker();\n"
"          } else {\n"
"            alert(\"Keine Koordinaten gefunden, bitte Adress-Anfrage ändern.\");\n"
"          }\n"
"        } \n"
"      };\n"
"      request.send();\n"
"    }\n"
"\n  "
"    // function to \"move\" marker (avoiding marker.moveTo(), as it is an \"unofficial\" function)\n"
"    function replaceMarker() {\n"
"      markers.clearMarkers();\n"
"      position = new OpenLayers.LonLat(longitude,latitude).transform(wgsProjection, mercatorProjection);\n"
"      marker = new OpenLayers.Marker(position);\n"
"      markers.addMarker(marker);\n"
"    }\n"
"\n  "
"\n  "
"    // function to update marker based on inputs and center on map\n"
"    function updateMarker() {\n"
"      longitude = document.getElementById(\"inputLon\").value;\n"
"      latitude = document.getElementById(\"inputLat\").value;  \n"
"\n  "
"      if (typeof map !== 'undefined') {\n"
"        if (latitude && longitude) {\n"
"          replaceMarker();\n"
"          map.setCenter(position, zoom);\n"
"        } else {\n"
"          markers.clearMarkers();\n"
"          map.setCenter(initialposition, initialzoom);\n"
"        }\n"
"      }\n"
"    }\n"
"\n  "
"    // function to \"move\" marker on click in map\n"
"    OpenLayers.Util.extend(touchNavigation, {\n"
"      defaultClick: function(evnt) {\n"
"        var lonlat = map.getLonLatFromViewPortPx(evnt.xy);\n"
"\n"
"        latitude =  lonlat.transform(mercatorProjection, wgsProjection).lat.toFixed(6);\n"
"        longitude = lonlat.lon.toFixed(6);\n"
"\n"
"        document.getElementById(\"inputLon\").value = longitude;\n"
"        document.getElementById(\"inputLat\").value = latitude;\n"
"\n"
"        replaceMarker();\n"
"      }\n"
"    });\n"
"\n"
"    // function to unfocus current form element when enter key was pressed\n"
"    function unfocusForm() {\n"
"      if (event.key == 'Enter') {\n"
"        document.activeElement.blur();\n"
"        return false;\n"
"      } else {\n"
"        return true;\n"
"      }\n"
"    }\n"
"\n  "
"    // execute the following once all DOM elements are loaded\n"
"    document.addEventListener('DOMContentLoaded', function(){\n"
"\n  "
//"      if(\"{{WIFICONNECTIVITY}}\"== \"true\"){\n"
"        // add geocoding button\n"
"        var findbutton = document.createElement(\"button\");\n"
"        findbutton.setAttribute(\"id\", \"geocode\");\n"
"        findbutton.setAttribute(\"type\", \"button\");\n"
"        findbutton.innerHTML = \"Suche Koordinaten\";\n"
"        findbutton.addEventListener (\"click\", function() {\n"
"          geolookup();\n"
"        });\n"
"        document.getElementById(\"pgeocode\").appendChild(findbutton);\n"
"\n  "
"        // init OpenLayers map\n"
"        map = new OpenLayers.Map(\"mapDiv\");\n"
"        map.addLayer(mapnik);\n"
"        map.addLayer(markers);\n"
"\n  "
"        updateMarker();\n"
"\n  "
"        // register function to \"move\" marker\n"
"        map.addControl(touchNavigation);   \n"
"        touchNavigation.activate();\n"
"\n  "
"        document.getElementById(\"inputLon\").addEventListener(\"change\", updateMarker);\n"
"        document.getElementById(\"inputLat\").addEventListener(\"change\", updateMarker);\n"
"\n  "
"\n  "
//"      }\n"
"    });\n"
"  </script>\n");

ACText(cfgData, " ");

ACElement(cfgBody,  
"<style type=\"text/css\"> \n"
"  input[type=number], input#owner { font-weight: 300; width: calc(100% - 124px); background-color: #fff; border: 1px solid #ccc; border-radius: 2px; color: #444; margin: 8px 0 8px 0; padding: 10px; }\n"
"  button[type=button] { font-weight: normal; padding: 8px 14px; margin: 12px; width: auto; outline: none; text-decoration: none; background-color: #1b5e20; border-color: #1b5e20; letter-spacing: 0.8px; color: #fff; border: 1px solid; border-radius: 2px; font-size: 0.9em; }\n"
"  div#OpenLayers_Control_Attribution_16 { bottom: .2em; }\n"
//    ".menu > a:link {"
//    "position: absolute;"
//    "display: inline-block;"
//    "right: 12px;"
//    "padding: 0 6px;"
//    "text-decoration: none;"
//    "}"
"</style>\n"
"\n"
//"<div class=\"menu\">" AUTOCONNECT_LINK(BAR_32) "</div>\n"
//"<h1 style=\"width: 90%\">Crowdsensor Konfiguration Firmware:FIRMWARE_VERSION</h1>\n"
//"  <label for \"inputFW\" style=\"width: 90%\">Crowdsensor Konfiguration Firmware: </label>\n"
//"  <input type=\"text\" name=\"inputFW\" id=\"inputFW\"  placeholder=\"FW\" style=\"border: none;background-color:none;outline:0;font-size: 50px;width: 1200px;height:50px; font-weight:bold;color:#000;\" readOnly value=\"\">\n"
//"\n"
"  <div id=\"mapDiv\" style=\"height:250px\"><br/></div><br/>\n"
"\n"
"  <h3>Standort des Sensors</h3>\n"
//"  <form>\n"
//"  <form onsubmit=\"return confirm('Sind die Daten korrekt?');\" onkeydown=\"return unfocusForm()\" action=\"/config\" method=\"POST\">\n"
"  <label for \"inputAddress\"><b>Adresse</b>, Format: Straße Hausnummer, Stadt</label>\n"
"  <br />\n"
"  <input type=\"text\" name=\"inputAddress\" id=\"inputAddress\" maxlength=\"160\" placeholder=\"Adresse\" value=\"\">\n"
"  <br />\n"
"  <span id=\"pgeocode\"></span>\n"
"  <br />\n"
"  <label for=\"inputLon\"><b>Länge</b>, Format: -3.703790 (für Europa im Bereich -15 bis 35):</label>\n"
"  <br />\n"
"  <input type=\"number\" name=\"inputLon\" id=\"inputLon\" min=\"-180\" max=\"180\" step=\"0.000001\" placeholder=\"z.B. -3.703790\" value=\"\" required onchange=\"updateMarker()\">\n"
"  <br />\n"
"  <br />\n"
"  <label for=\"inputLat\"><b>Breite</b>, Format: 40.416775 (für Europa im Bereich 35 bis 75):</label>\n"
"  <br />\n"
"  <input type=\"number\" name=\"inputLat\" id=\"inputLat\" min=\"-90\" max=\"90\" step=\"0.000001\" placeholder=\"z.B. 40.416775\" value=\"\" required onchange=\"updateMarker()\"> \n"
"  <br />\n"
"  <br />"
"  <h3>Sensoren</h3>\n"
"  <label for=\"rht_sensor\"><b>Temperatursensor</b>, der benutzt werden soll</label>\n"
"  <br />\n"
"  <select name=\"rht_sensor\" id=\"rht_sensor\">\n"
"  <option value=\"default\">(keiner)</option>\n"
"  <option value=\"BME280\">BME280</option>\n"
"  <option value=\"DHT22\">DHT22</option>\n"
"  </select>\n"
"  <br />\n"
"  <br />\n"
"  <label for=\"sds011id\"><b>ID des SDS011</b> Feinstaub-Sensors (XXXX-XXXX)</label>\n"
"  <br />\n"
"  <input type=\"text\"  name=\"sds011id\" id=\"sds011id\" maxlength=\"10\" placeholder=\"XXXX-XXXX\" value=\"\">\n"
"  <br />\n"
"  <br />\n"
"  <h3>Datenlizenz</h3>\n"
"  <label for=\"data_license\"><b>Lizenz</b>, unter der die von deinem Sensor aufgenommenen Daten im SmartAQnet veröffentlicht werden sollen</label>\n"
"  <br />\n"
"  <select name=\"data_license\" id=\"license\" onchange=\"set_required()\">\n"
"  <option value=\"CC0 1.0\">CC0 1.0 Universell</option>\n"
"  <option value=\"CC BY 4.0\">CC BY 4.0 Namensnennung International</option>\n"
"  </select>\n"
"  <br />\n"
"  <span style=\"font-size: 75%\"> Weitere Informationen: <a href=\"" data1_url_cc0 "\" id=\"license_url\">" data1_url_cc0 "</a></span>\n"
"  <script>\n"
"  function set_required() {\n"
"    var lic = document.getElementById('license').value;\n"
"      if (lic == \"CC BY 4.0\") {\n"
"        document.getElementById(\"owner\").required = true;\n"
"        document.getElementById(\"license_url\").innerHTML = \"" data1_url_ccby "\";\n"
"        document.getElementById(\"license_url\").href = \"" data1_url_ccby "\";\n"
"      } else {\n"
"        document.getElementById(\"owner\").required = false;\n"
"        document.getElementById(\"license_url\").innerHTML = \"" data1_url_cc0 "\";\n"
"        document.getElementById(\"license_url\").href = \"" data1_url_cc0 "\";\n"
"      }\n"
"  }\n"
"  </script>\n"
"  <br />\n"
"  <br />\n"
"  <label for=\"data_owner\"><b>Name</b> für Namensnennung in Lizenz (optional bei CC0)</label>\n"
"  <br />\n"
"  <input type=\"text\" name=\"data_owner\" id=\"owner\" maxlength=\"40\" placeholder=\"Name (optional bei CC0)\" value=\"\">\n"
//"  <br />\n"
//"  <br />\n"
"  <input type=\"button\" value=\"Speichern1\" name=\"cfgButton1\" onclick = \"if (confirm('Daten speichern?')) _sa('/config')\" method=\"post\">\n"
//"  <input type=\"submit\" value=\"Speichern\" onclick = \"ConfirmBox1();\" method=\"post\">\n"
//"  <input type=\"submit\" value=\"Speichern\" onclick = \"return confirm('Daten speichern?');\" window.location.href=\"_sa('/config')\">\n"
//"  <form onsubmit=\"return confirm('Sind die Daten korrekt?');\" onkeydown=\"return unfocusForm()\" action=\"/config\" method=\"POST\">\n"
//"  <input type=\"submit\" value=\"Speichern\">\n"
//"</form>\n"
"<script>\n"
"function ConfirmBox1() {\n"
"  if (confirm('Daten speichern1?')) window.location.href = \"/config\";\n"
"}\n"
"</script>\n"
"<br />\n"
);
ACElement(cfgBody2,
"<p>Hinweise: <br />\n"
"<ul>\n"
"  <li>Die Daten werden nur gespeichert, wenn sie den \"Speichern\" Button klicken.</li>\n"
"  <li>Die Eingabe neuer Daten überschreibt die alten Daten (z.B. für Standortwechsel).</li>\n"
"  <li>Alle von Ihnen eingegeben Informationen werden offen in der Datenbank abrufbar sein.</li>\n"
"</ul>\n"
"</p>\n"
"<script>\n"
"  window.addEventListener('load', function(){\n"
"    document.getElementById(\"_aux\").onkeydown = \"return unfocusForm()\"\n"
"  });\n"
"</script>\n"
"");

const char* scConfirmBox = R"(
<script>
function ConfirmBox() {
  if (confirm("Daten speichern?")) 
  window.location.href = "/config";
}
</script>
)";

//ACButton(cfgButton, "Speichern", "ConfirmBox()");
ACSubmit(cfgButton, "Speichern", "/config");
ACElement(cfgScript2, scConfirmBox);
ACInput(channelid, "", "Channel ID");
ACText(parameters3, "", "text-align:center;color:#2f4f4f;");

AutoConnectAux cfgSensorPageAux("/", "Configure", true, {
  parameters3,
  cfgScript,
  cfgBody,
  cfgData,
  cfgButton,
  cfgScript2,
  cfgBody2,
  channelid
});

ACElement(cfgScript3, otaUpdate);
AutoConnectAux cfgUpdatePageAux("/otaUpdate", "Update", true, {
  cfgScript3
});

void handleClearChannel() {
  HTTPClient  httpClient;
  WiFiClient  client;
  Serial.println(" failed");

  /*inputAddress.value.trim();
  inputLon.value.trim();
  inputLat.value.trim();
  rht_sensor.value.trim();
  sds011id.value.trim();
  license.value.trim();
  owner.value.trim();
*/
  
  // Returns the redirect response.
  WebServer&  webServer = Portal.host();
  /*   
failed
Sajjad Hussain1223880
8.424171
Crowdsensor Konfiguration Firmware: 0.8.6.1 pre
Sajjad Hussain1223880
8.424171
Crowdsensor Konfiguration Firmware: 0.8.6.1 pre
Sajjad Hussain1223880
Sajjad Hussain1223880
*/
  Serial.println(Server.arg("channelid"));
  Serial.println(Server.arg("inputLat"));
Serial.println(Server.arg(0));
  Serial.println(webServer.arg("channelid"));
  Serial.println(webServer.arg("inputLat"));
Serial.println(webServer.arg(0));

cfgSensorPageAux.fetchElement();    // Preliminary acquisition

AutoConnectInput& abc = cfgSensorPageAux["channelid"].as<AutoConnectInput>();
Serial.println(abc.value);
Serial.println(cfgSensorPageAux["channelid"].value);

    
  webServer.sendHeader("Location", String("http://") + webServer.client().localIP().toString() + String("/"));
  webServer.send(302, "text/plain", "");
  webServer.client().flush();
  webServer.client().stop();
}

String handleClearChannel1(AutoConnectAux& aux, PageArgument& args) {

  Serial.println(" failed1");
  AutoConnectAux&   input_page = *Portal.aux("/");
  AutoConnectInput& abc = input_page["channelid"].as<AutoConnectInput>();
Serial.println(abc.value);
  
      return String(); 
}


String refreshParams(AutoConnectAux& aux, PageArgument& args) {

    String tempLicense = get_value("user_data", "data_license");
    cfgTemp = "<script> \n"
               "  document.addEventListener('DOMContentLoaded', function(){\n"
               //"    document.getElementById(\"inputFW\").value =\"Crowdsensor Konfiguration Firmware: " + String(FIRMWARE_VERSION) + "\";\n"
               "    document.getElementById(\"inputAddress\").value =\"" + get_value("user_data", "address") + "\";\n"
               "    document.getElementById(\"inputLon\").value =\"" + get_value("user_data", "longitude") + "\";\n"
               "    document.getElementById(\"inputLat\").value =\"" + get_value("user_data", "latitude") + "\";\n"
               "    document.getElementById(\"rht_sensor\").value =\"" + get_value("user_data", "rht_sensor") + "\";\n"
               "    document.getElementById(\"sds011id\").value =\"" + get_value("user_data", "sds011id") + "\";\n"
               "    document.getElementById(\"license\").value =\"" + ((tempLicense == NULL) ? "CC0 1.0" : tempLicense) + "\";\n"
               "    document.getElementById(\"owner\").value =\"" + get_value("user_data", "data_owner") + "\";\n"
               "  });\n"
               "  window.addEventListener('load', function(){\n"
               "    updateMarker();"
               "  });\n"
               "</script>";

#ifndef RELEASE
    Serial.println(cfgTemp);
#endif

    AutoConnectText& ac_scriptdiv = aux.getElement<AutoConnectText>("cfgData");
    ac_scriptdiv.value = cfgTemp.c_str();
    return String();  
  }
  void otaUpdateHandler(void)
  {
    Server.sendHeader("Connection", "close");
    Server.send(200, "text/html", otaUpdate);
  }
  void updateHandler(void)
  {
    Server.sendHeader("Connection", "close");
    Server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }
  void updateHandlerResponse(void)
  {
    WebServer* _server = &Server;
    HTTPUpload& upload = _server->upload();
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
        delay(100);
        _server->client().stop();
        ESP.restart();
      } else {
        Update.printError(Serial);
      }
    }
  }
// -----------------------------------------------------------------------------
