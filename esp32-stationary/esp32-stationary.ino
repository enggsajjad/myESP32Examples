//#define RELEASE // uncomment to switch servers to release version, enable sending to madavi and luftdaten.info, and supress some debug output
#define FIRMWARE_VERSION "0.8.6 pre" // (for intermediate versions being actively worked on, append "pre" to number, indicating that it is "not yet" the specified number)
/*
    ==change history (since version 0.8)==
    ===0.8.1===
    * bugfix for coordinates: switched to coordinates[]={long, lat...}
    ===0.8.2===
    * added hack for HTTP patch for location
    * removed all libs from src folder that are used in its original state
      and included the libs directly in arduino
    * updated SDS011 lib to newest version
    * changed display font
    * added over the air update support
    * added mDNS: now esp32.local can be typed into browser to connect to esp32
    === 0.8.3 ===
    * bugfix for error that no reboot occured after timeout in ap mode
    * bugfix for wrong display message if no RH T sensor is configured
    * bugfix for wrong display message if Wifi is searched
    * improved display messages
    * bugfix for not working geocode button
    === 0.8.4 ===
    * added properties and webinterface inputs for data license
    * geocoding button does not work again, previous bugfix did not work
    * [...]  
    === 0.8.5 ===
    * (hopefully) permanent bugfix for not working geocode button
    * added license url to webinterface 
    * added RELEASE macro that can be used to switch between development and release versions (switches server addresses and supressses some serial output)
    * display shows FW version for 2 sec on startup 
    * webinterface shows map to pick location
    * added reboot after saving configuration, so that thing and datastreams are patched in new setup()
    *

    ==Copyright==:
    This ino file is by TECO (www.teco.edu).
    The following libraries were used:
    ===Espressif Systems===
    Most of the code is based on code from Espressif Systems.
    Espressif Systems:
    Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

    ===U8g2lib.h & U8x8lib.h===
    Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

    Copyright (c) 2016, olikraus@gmail.com
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

      Redistributions of source code must retain the above copyright notice, this list
      of conditions and the following disclaimer.

      Redistributions in binary form must reproduce the above copyright notice, this
      list of conditions and the following disclaimer in the documentation and/or other
      materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
    CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
    STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
    ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


    ===AutoConnect library===
    Autoconnect part is based on Simple.ino, Example for the AutoConnect library.
    released under the MIT License: https://opensource.org/licenses/MIT
    Copyright (c) 2018, Hieromon Ikasamo
    https://github.com/Hieromon/AutoConnect


    ===ArduinoJson===
    Released under the MIT License: https://opensource.org/licenses/MIT
    Copyright © 2014-2018 Benoit BLANCHON


    ===PageBuilder===
    Released under the MIT Licence: https://opensource.org/licenses/MIT
    Copyright (c) 2017-2019 Hieromon Ikasamo


    ===MIT Licence===
    Permission is hereby granted, free of charge, to any person obtaining a copy of
    this software and associated documentation files (the “Software”), to deal in
    the Software without restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
    Software, and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
    IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.




    ==general==
    This script uses ESP32 HELTEC Board, SDS011 Sensor and a BME280 sensor or a DHT22 sensor.
    After power on, it opens a Wifi AP, Password is 12345678.
    A Captive Portal opens after connection to this AP with Smartphone.
    Position data, sensor and Wifi AP can be selected.

    ==prerequisites==

    === board ===
    esp32 board by espressiv systems v.1.0.1
    change builtin library and replace the files  libraries/HTTPClient/src/HTTPClient.cpp and
    libraries/HTTPClient/src/HTTPClient.h with the files of this commit:
    https://github.com/espressif/arduino-esp32/pull/2610/commits/357ed760ee4a516170a4eaa1e75f36b59431d6a9
    

    ===libraries===:
    
    Adafruit Unified Sensor by Adafruit v1.0.3 (https://github.com/adafruit/Adafruit_Sensor)
    Adafruit BME280 Library by Adafruit v1.0.8 (https://github.com/adafruit/Adafruit_BME280_Library)
    ArduinoJson by Benuit Blanchon v 5.13.5 (https://arduinojson.org/?utm_source=meta&utm_medium=library.properties) (IMPORTANT: ArduinoIDE installs the newest beta version (v6), but it doesnt work with the newest version.)
    DHT sensor library by Adafruit v1.3.4 (https://github.com/adafruit/DHT-sensor-library)
    PageBuilder v1.3.1 by Hieromon Ikasamo (https://github.com/Hieromon/PageBuilder)
    eztime by Rop Gonggrijp v0.7.9 (https://github.com/ropg/ezTime)
    u8g2 by oliver v2.25.10 (https://github.com/olikraus/u8g2)
    
    Autoconnect lib (is already in src folder, no need to install) [MODIFIED]
    SDS011 sensor Library by R. Zschiegner (already in src folder, no need to install) [MODIFIED]



    ==wiring==

    ===SDS===
    connect TXD of Dust Sensor SDS011 to Pin 13 of ESP32 (ESP-RX). If it doesnt work, check if pin change was executed (see main file =change pins=).
    connect 5V of Dust Sensor SDS011 to Pin 5V of ESP32.
    connect GND of Dust Sensor SDS011 to Pin GND of ESP32.

    ===BME===
    connect SCK of BME280 sensor to Pin 22 of ESP32 (ESP-SCL)
    connect SDA/SDI of BME280 sensor to Pin 21 of ESP32 (ESP-SDA)
    connect VCC of BME280 sensor to 3V3 of ESP32
    connect GND of BME280 sensor to GND of ESP32

    ===DHT===
    Connect pin 1 of DHT22 sensor (on the left) to 3V3 of ESP32
    Connect pin 2 of DHT22 sensor to Pin 21 of ESP32 (data)
    Connect pin 4 of DHT22 sensor (on the right) to GND of ESP32
    Connect a resistor in range 4.7k to 10k from pin 2 (data) to pin 1 (power) of the sensor



    ==troubleshooting==
    if the storage space is too small when compiling, change it in arduino IDE:
    Tools -> board -> select ESP32 DEV Module
    Tools -> partition scheme -> No OTA (large APP)

    ==NVS==
    ESP32 key value store with Preferences library.
    uses the Preferences library to store key value pairs in
    non-volatile storage on ESP32 processor.
    created for arduino-esp32 09 Jul 2018
    by Felix Ernst (ernst(at)teco.edu)

    ==Autoconnect==
    allows to connect to the ESP via Wifi and then change to AP to which the ESP connects
    default ssid of ESP: esp32ap
    default wifi-pw: 12345678
  /
    ==Auto reboot"
    The device reboots after a certain time (see watchdog.h)


*/
// constants

#define ENABLE_SEND2FROST true // switch for sending to FROST teco server
#define SENSOR_INTERVAL 10000 // intervall of reading sensor data in ms
#ifdef RELEASE
  #define ENABLE_SEND2MADAVI true // switch for sending to madavi api 
  #define ENABLE_SEND2LUFTDATEN true // switch for sending to luftdaten api 
#else
  #define ENABLE_SEND2MADAVI false // switch for sending to madavi api 
  #define ENABLE_SEND2LUFTDATEN false // switch for sending to luftdaten api 
#endif
  #define SEND2MADAVI_INTERVAL 145000// intervall of sending to madavi api in ms 
  #define SEND2LUFTDATEN_INTERVAL 145000// intervall of sending to madavi api in ms 
#define ENABLE_DAILY_REBOOT true //enable daily reboot at specific time
#define TIMETOREBOOT "0400" // set time when to do daily reboot (hour of time of day for reboot goes here) //TODO set proper time
#define ENABLE_APMODE_REBOOT true // if true, automatic reboot after REBOOT_TIMEOUT is enabled
#define APMODE_REBOOT_TIMEOUT 36000000 // intervall of auto reboot while in access point mode (config mode) in ms (36000000ms is 10min)
// autoreboot while in ap mode only occurs if no device is connected to the esp32


// host to send data
//char destination_madavi[] = "http://192.168.12.67/data.php"; //TODO set proper url
//char destination_luftdaten[] = "http://192.168.12.67/data.php"; //TODO set proper url
char destination_madavi[] = "https://api-rrd.madavi.de/data.php";
char destination_luftdaten[] = "https://api.luftdaten.info/v1/push-sensor-data/";

#include "src/lib/SDS011/SDS011.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#include <ArduinoJson.h>
//#include <sstream> //c++ libary to use std::string
//#include <sdkconfig.h>
#include <Preferences.h>
//#include "Arduino.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "src/lib/AutoConnect/src/AutoConnect.h"
#include "src/lib/AutoConnect/src/AutoConnectCredential.h"
#include <PageBuilder.h>
#include <U8x8lib.h>
#include <time.h>
//#include <WiFiUdp.h>
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


// do not change this vars
// used to store mac address
byte mac[6];
String esp32_chipid;
bool use_bme280 = false;
bool use_dht22 = false;
//bool apmode = false;
bool initialised_frost = false;
bool wifi_connected;
bool enable_send2frost = ENABLE_SEND2FROST;
bool enable_send2madavi = ENABLE_SEND2MADAVI;
bool enable_send2luftdaten = ENABLE_SEND2LUFTDATEN;
bool time_updated;



// initialise the OLED which is used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

// initialise udp for network time
WiFiUDP ntpUDP;

WebServer Server; // initialise Webserver
AutoConnect Portal(Server); // initialise AutoConnect
AutoConnectConfig  Config;


// dynamic strings and getter for html content
String viewCredential(PageArgument&);
String changeCfg(PageArgument&);
String getAddress(PageArgument&);
//String getStreet(PageArgument&);
//String getStreetnumber(PageArgument&);
//String getCity(PageArgument&);
//String getCountry(PageArgument&);
String getLongitude(PageArgument&);
String getLatitude(PageArgument&);
String getHeightNN(PageArgument&);
String getHeightGround(PageArgument&);
String getRhtSensor(PageArgument&);
String getSds011Id(PageArgument&);
String getDataOwner(PageArgument&);
String getDataUrlCC0(PageArgument&);
String getDataUrlCCBY(PageArgument&);
String getFirmwareVersion(PageArgument&);
//String getTimeTilReboot(PageArgument&);
//String getApMode(PageArgument&);
String getWifiConnectivity(PageArgument&);

static String data_url_cc0 = "https://creativecommons.org/publicdomain/zero/1.0/deed.de";
static String data_url_ccby = "https://creativecommons.org/licenses/by/4.0/deed.de";



// Specified the offset if the user data exists.
// The following two lines define the boundalyOffset value to be supplied to
// AutoConnectConfig respectively. It may be necessary to adjust the value
// accordingly to the actual situation.

#define CREDENTIAL_OFFSET 0
//#define CREDENTIAL_OFFSET 64

/**
    An HTML for the operation page.
    In PageBuilder, the token {{SSID}} contained in an HTML template below is
    replaced by the actual SSID due to the action of the token handler's
   'viewCredential' function.
    The position can be inserted into html form and is then saved to a String
*/

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


static const char PROGMEM autoconnectMenu[] = { AUTOCONNECT_LINK(BAR_24) };

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




// page builder elements
PageBuilder rootPage("/", { elmList });
PageElement elmCfg("{{DEL}}", {{ "DEL", changeCfg }});
PageBuilder cfgPage("/config", { elmCfg });



// Custom methods to retrieve stored data for insertion into HTML page

String getAddress(PageArgument& args) {
  return String(get_value("user_data", "address"));
}

String getLongitude(PageArgument& args) {
  return String(get_value("user_data", "longitude"));
}

String getLatitude(PageArgument& args) {
  return String(get_value("user_data", "latitude"));
}

String getDataOwner(PageArgument& args) {
  return String(get_value("user_data", "data_owner"));
}

String getDataUrlCC0(PageArgument& args) {
  return data_url_cc0;
}

String getDataUrlCCBY(PageArgument& args) {
  return data_url_ccby;
}

String getFirmwareVersion(PageArgument& args) {
  return F(FIRMWARE_VERSION);
}

String getRhtSensor(PageArgument& args) {
  String tmp = String(get_value("user_data", "rht_sensor"));
  if (tmp == "") {
    return "keiner";
  }
  else {
    return tmp;
  }
}

String getSds011Id(PageArgument& args) {
  return String(get_value("user_data", "sds011id"));
}


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




// Retrieve the credential entries from EEPROM, Build the SSID line
// with the <li> tag.
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

// save formular data in eeprom
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




String getIsoTime() {
  return (UTC.dateTime("Y-m-d\\TH:i:s") + ".000Z");
}

String getHourMinute() {
  return (UTC.dateTime("Hi"));
}

String getChipId() {
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



/*
  FROST manager
*/
// create frostmanager instance
//char latitude[
//float latitude = atof((get_value("user_data", "latitude")).toCharArray());


//float coordinates[] = {latitude, longitude, abovenn, aboveground};


// frostManager(String esp32_chipId, String SDS_SerialNumber, <which temp sensor to use>, String Temp-sensor_SerialNumber, float coordinates[]={longitude, latitude, abovenn, aboveground}, String human-readable-location)
//instance of FrostManager class and constructor

FrostManager *frostManager;

LicenseProperty* mylicenseproperty;

/*
   Setup part
*/

void setup() {

  // make serial monitor printing available
  Serial.begin(115200);

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
  if (!(license_type == "")) {
    String license_owner = get_value("user_data", "data_owner");
    String license_url = get_value("user_data", "data_url");
    mylicenseproperty = new LicenseProperty(license_type,license_owner,license_url);
#ifndef RELEASE
    Serial.println(license_type + " " + license_owner + " " + license_url);
#endif
  }

  


  // setup display
  u8x8.begin();
//  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFont(u8x8_font_torussansbold8_r);

  // print FW version on display for 2 seconds
  u8x8.print(F("FW: "));
  u8x8.print(FIRMWARE_VERSION);
  delay(2000);
  u8x8.clearDisplay();

  //  messages on display
  u8x8.drawString(0, 7, "ID: ");
  u8x8.drawString(4, 7, esp32_chipid.c_str());

  sds.begin(&SDS_SERIAL, 13, 17);  // initialize SDS011 sensor

#ifndef RELEASE
    Serial.print("[Setup] Initializing temperature sensor ");
#endif
  // start bme280 if chosen in webinterface
  if (get_value("user_data", "rht_sensor") == "BME280")
  {
#ifndef RELEASE
    Serial.print("BME280... ");
#endif
    use_bme280 = bme.begin(); // initialise BME280 and save state to a var
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
    //    use_dht22 = true;
#ifndef RELEASE
    Serial.println(" done. ");
#endif
  }



  /*
     autoconnect to wifi and captiveportal
  */
  rootPage.insert(Server);    // Instead of Server.on("/", ...);
  //  cfgSavedPage.insert(Server);    // Instead of Server.on("/", ...);
  cfgPage.insert(Server);     // Instead of Server.on("/del", ...);
  Server.on("/_ac/otaUpdate", HTTP_GET, []() {
    Server.sendHeader("Connection", "close");
    Server.send(200, "text/html", otaUpdate);
  });
    /*handling uploading firmware file */
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
      /* flashing firmware to ESP*/
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
  



  Config.title = "TECO Crowdsensor";
  // end sending AP after timeout, go into loop
  Config.portalTimeout = APMODE_REBOOT_TIMEOUT; 
  Config.boundaryOffset = CREDENTIAL_OFFSET; 
  // use last 6 digits of mac address for ssid (or apid)
  Config.apid = esp32_chipid.c_str();
  Config.psk = "12345678";
  Portal.config(Config);

  // Start
  if (Portal.begin()) {
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
    u8x8.clearDisplay();
    //    while (!timeClient.update()){
    //      timeClient.forceUpdate();
    //    }
   
  }

  if (WiFi.status() == WL_CONNECTED) {
    u8x8.drawString(0, 0, "WARTE AUF ");
    u8x8.drawString(0, 1, "ZEIT-SYNC");
  
  
    Serial.println("wait for network time sync start");
    // update time from network
    // true if update was successful
    time_updated = waitForSync();
    Serial.println("got network time");
    u8x8.clearDisplay();
  }




}

/*
   loop
*/

void loop() {
//
//  String resultentity;
//
//  resultentity = frostManager->getEntity();
//  delay(10000);

  //  Serial.print("time updated?: ");
  //  Serial.println(time_updated);
  bool enable_daily_reboot = ENABLE_DAILY_REBOOT;


  
  String timetoreboot = TIMETOREBOOT;


  //reboot daily at specific time
    if (time_updated && enable_daily_reboot){
      if ((getHourMinute() == timetoreboot) && (millis() > 60000)){
        resetModule();
        }
      }

  // vars to store sensor results
  float pm10, pm25; // sensor results
  float temp;
  float hum;
  float atm;
  String timeofmeas_sds;
  String timeofmeas_rht;

  int status_sds;



  // dont use delay since everything is blocked then. millis() return the milliseconds after
  // start of the program, so trigger an event once every 1000ms.
  // millis overflow occurs after 50 days (var goes back to 0), but this doesnt matter
  // since we work with a substraction



  // if connected to wifi, do measurement and send data to apis
  // check if network time update was successful, if not dont send data
  if (WiFi.status() == WL_CONNECTED) {
    u8x8.drawString(0, 0, "VERBUNDEN MIT:");
    u8x8.drawString(0, 1, WiFi.SSID().c_str());
    u8x8.drawString(0, 2, WiFi.localIP().toString().c_str());




    // check if time since last measurement is greater than sensor_interval
    if ((unsigned long)(millis() - previousMillis_sensor) >= sensor_interval) {
      previousMillis_sensor = millis();
      //    Serial.println(millis());




      // read sensor and store results into global variables pm10, pm25
      // is 1 if failed, and 0 if suceeded
      status_sds = sds.read(&pm25, &pm10);
      timeofmeas_sds = getIsoTime();



      // Serial.print("use_dht: ");
      //Serial.println(use_dht22);
      //        Serial.print("use_bme: ");
      //        Serial.println(use_bme280);


      // if bme280 sensor is connected, read values
      if (get_value("user_data", "rht_sensor") == "BME280") {
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
        if (isnan(hum)) {
          use_bme280 = false;
        }
      } else if (get_value("user_data", "rht_sensor") == "DHT22") {
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
        if (isnan(hum)) {
          use_dht22 = false;
        }
      }
      //      char *data_madavi;
      //      data_madavi = json_madavi(status_sds, pm10, pm25, temp, hum, atm);
      //      Serial.println(json_madavi(status_sds, pm10, pm25, temp, hum, atm));

      //      Serial.println(timeClient.getFormattedTime());
      //      Serial.println(data_json_madavi_char);

      /*
        printing info to display
      */
      char pm25_char[6];
      char pm10_char[6];
      char temp_char[6];
      char hum_char[6];
      if (!status_sds) {
        u8x8.drawString(0, 3, "PM2.5: ");
        dtostrf(pm25, 5, 1, pm25_char); // convert to string for displaying
        u8x8.drawString(7, 3, pm25_char);
        u8x8.drawString(0, 4, "PM10:  ");
        dtostrf(pm10, 5, 1, pm10_char);
        u8x8.drawString(7, 4, pm10_char);
      }
      if (status_sds) {
        u8x8.drawString(0, 3, "FEINSTAUBSENSOR ");
        u8x8.drawString(0, 4, ":Bitte verbinden");
      }
      if ((use_bme280 && !isnan(temp)) || (use_dht22 && !isnan(temp))) {
        dtostrf(temp, 5, 1, temp_char);
        //          Serial.print("temp_vor_char");
        //          Serial.println(temp);
        //          Serial.print("temp_char");
        //          Serial.println(temp_char);
        u8x8.drawString(0, 5, "Temp:  ");
        u8x8.drawString(7, 5, temp_char);
        dtostrf(hum, 5, 1, hum_char);
        u8x8.drawString(0, 6, "Humid: ");
        u8x8.drawString(7, 6, hum_char);
        u8x8.drawString(14, 6, "%");
      }

      if (!use_bme280 && !use_dht22) {
        Serial.println("sensoren: ");
        Serial.println(String(get_value("user_data", "rht_sensor")));
        if ((String(get_value("user_data", "rht_sensor")) == "") || (String(get_value("user_data", "rht_sensor")) == "keiner")){
          u8x8.drawString(0, 5, "TEMPERATURSENSOR");
          u8x8.drawString(0, 6, "Bitte auswaehlen");
        }
        else {
        u8x8.drawString(0, 5, "TEMPERATURSENSOR");
        u8x8.drawString(0, 6, ":Bitte verbinden");
        }
      }
      u8x8.drawString(0, 7, "ID: ");
      u8x8.drawString(4, 7, esp32_chipid.c_str());


      // only send if position data is given
      if (enable_send2frost &&  (get_value("user_data", "latitude") != ""))
      {
        Serial.println("position data is given, start sending");

        if (!time_updated) // if no time sync, then retry
        {
          time_updated = waitForSync();
        }
        // in case timesync is true, send to frost
        else {



          //send data to FROST
          if (!initialised_frost) {
            // initialse frost manager
            Serial.println("initialze FROST manager");
            frostManager->setDatastreamProperty(mylicenseproperty);
            frostManager->createEntities();
            initialised_frost = true;
          }


          // send as often as a measurement occurs

          // send SDS data
          if (!status_sds) {
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

          // send hum and temp data

          // send bme280 data
          if (use_bme280 && !isnan(temp)) {
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

          // send dht22 data
          if (use_dht22 && !isnan(temp)) {
            {
              Observation observation(frostManager->dataStreamTemperatureDHT22_Id, timeofmeas_rht, timeofmeas_rht, temp);
              frostManager->postObservation(&observation);
            }
            {
              Observation observation(frostManager->dataStreamHumidityDHT22_Id, timeofmeas_rht, timeofmeas_rht, hum);
              frostManager->postObservation(&observation);
            }
          }
        }
      }
    }




    // send to madavi api after send2madavi_intervall milliseconds
    if (((unsigned long)(millis() - previousMillis_send2madavi) >= send2madavi_interval)  && !status_sds && enable_send2madavi)
    {
      previousMillis_send2madavi = millis();
      // send all data to madavi api
      Serial.println("sending all data to madavi");
      // send data to madavi
      sendData(json_madavi(status_sds, pm10, pm25, temp, hum, atm).c_str(), destination_madavi, "0");

    }


    // send single sensor data to luftdaten api after send2luftdaten_intervall milliseconds
    if (((unsigned long)(millis() - previousMillis_send2luftdaten) >= send2luftdaten_interval) && !status_sds && enable_send2luftdaten)
    {
      previousMillis_send2luftdaten = millis();
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
    }




  }

  else if (WiFi.status() == WL_CONNECT_FAILED) {
    u8x8.drawString(0, 0, "VERBINDUNG      ");
    u8x8.drawString(0, 1, "FEHLGESCHLAGEN  ");
    u8x8.drawString(0, 2, "STARTE NEU...   ");
    u8x8.clearLine(3);
    u8x8.clearLine(4);
    u8x8.clearLine(5);
    u8x8.clearLine(6);
    u8x8.drawString(0, 7, "ID: ");
    u8x8.drawString(4, 7, esp32_chipid.c_str());
    delay(5000);
    resetModule();
  }

  else if (WiFi.status() == WL_CONNECTION_LOST) {
    u8x8.drawString(0, 0, "VERBINDUNG      ");
    u8x8.drawString(0, 1, "FEHLGESCHLAGEN  ");
    u8x8.drawString(0, 2, "STARTE NEU...   ");
    u8x8.clearLine(3);
    u8x8.clearLine(4);
    u8x8.clearLine(5);
    u8x8.clearLine(6);
    u8x8.drawString(0, 7, "ID: ");
    u8x8.drawString(4, 7, esp32_chipid.c_str());
    delay(5000);
    resetModule();
  }

  else if (WiFi.status() == WL_DISCONNECTED) {
    u8x8.drawString(0, 0, "VERBINDUNG      ");
    u8x8.drawString(0, 1, "FEHLGESCHLAGEN  ");
    u8x8.drawString(0, 2, "STARTE NEU...   ");
    u8x8.clearLine(3);
    u8x8.clearLine(4);
    u8x8.clearLine(5);
    u8x8.clearLine(6);
    u8x8.drawString(0, 7, "ID: ");
    u8x8.drawString(4, 7, esp32_chipid.c_str());
    delay(5000);
    resetModule();
  }







  Portal.handleClient();


}
