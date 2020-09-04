/*
  HandleCline.ino, Example for the AutoConnect library.
  Copyright (c) 2018, Hieromon Ikasamo
  https://github.com/Hieromon/AutoConnect

  This software is released under the MIT License.
  https://opensource.org/licenses/MIT
*/

#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
#endif
#include <AutoConnect.h>

#if defined(ARDUINO_ARCH_ESP8266)
ESP8266WebServer server;
#elif defined(ARDUINO_ARCH_ESP32)
WebServer server;
#endif

#ifndef BUILTIN_LED
#define BUILTIN_LED  2  // backward compatibility
#endif

AutoConnect         portal(server);
unsigned char bmeAddress =0x77;
void handleRoot() {
  String page = PSTR(
"<html>"
"</head>"
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
  "<style type=\"text/css\">"
    "body {"
    "-webkit-appearance:none;"
    "-moz-appearance:none;"
    "font-family:'Arial',sans-serif;"
    "text-align:center;"
    "}"
    ".menu > a:link {"
    "position: absolute;"
    "display: inline-block;"
    "right: 12px;"
    "padding: 0 6px;"
    "text-decoration: none;"
    "}"
    ".button {"
    "display:inline-block;"
    "border-radius:7px;"
    "background:#1b5e20;"
    "margin:0 10px 0 10px;"
    "padding:10px 20px 10px 20px;"
    "text-decoration:none;"
    "color:#fff;"
    "}"
  "</style>"
"</head>"
"<body>"
  "<div class=\"menu\">" AUTOCONNECT_LINK(BAR_24) "</div>"
  "<h3>INTERNAL TESTING</h3><br>"
  "<hr>"
  "To set the BME280 I2C Address, use the following buttons.<br>");
  page += String(F("<span style=\"font-weight:normal;color:"));
  page += String("Black\" >Current Address is:");
  page += String(F("</span>"));
  page += String(F("<span style=\"font-weight:bold;color:"));  
  page += (bmeAddress == 0x77) ? String("Red\">Red=0x77") : String("Purple\">Purple=0x76");
  page += String(F("</span>"));
  page += String(F("<p><a class=\"button\" href=\"/io?v=0x76\">Purple</a><a class=\"button\" href=\"/io?v=0x77\">Red</a></p>"));
  
  page += String(F("<hr>"));
  page += String(F("<span style=\"font-weight:normal;color:"));
  page += String("Black\" >To set statistics to initial values, use the following button.");
  page += String(F("</span>"));
  page += String(F("<p><a class=\"button\" href=\"/io?v=0x17\">Init</a></p>"));
  page += String(F("<hr>"));
  page += String(F("</body></html>"));
  server.send(200, "text/html", page);
}

void handleGPIO() {
  if (server.arg("v") == "0x76")
  {
    digitalWrite(BUILTIN_LED, LOW);
    bmeAddress =0x76;
    Serial.println("0x76");
  } 
  else if (server.arg("v") == "0x77")
  {
    digitalWrite(BUILTIN_LED, HIGH);
    bmeAddress =0x77;
    Serial.println("0x77");
  }
  else if (server.arg("v") == "0x17")
  {
    
    Serial.println("init");
    
  }
  sendRedirect("/");
}

void sendRedirect(String uri) {
  server.sendHeader("Location", uri, true);
  server.send(302, "text/plain", "");
  server.client().stop();
}

bool atDetect(IPAddress softapIP) {
  Serial.println("Captive portal started, SoftAP IP:" + softapIP.toString());
  return true;
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  pinMode(BUILTIN_LED, OUTPUT);

  // Put the home location of the web site.
  // But in usually, setting the home uri is not needed cause default location is "/".
  //portal.home("/");   

  server.on("/", handleRoot);
  server.on("/io", handleGPIO);

  // Starts user web site included the AutoConnect portal.
  portal.onDetect(atDetect);
  if (portal.begin()) {
    Serial.println("Started, IP:" + WiFi.localIP().toString());
  }
  else {
    Serial.println("Connection failed.");
    while (true) { yield(); }
  }
}

void loop() {
  server.handleClient();
  portal.handleRequest();   // Need to handle AutoConnect menu.
  if (WiFi.status() == WL_IDLE_STATUS) {
#if defined(ARDUINO_ARCH_ESP8266)
    ESP.reset();
#elif defined(ARDUINO_ARCH_ESP32)
    ESP.restart();
#endif
    delay(1000);
  }
}
