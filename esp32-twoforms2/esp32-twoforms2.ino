#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>

static const char input_page[] PROGMEM = R"raw(
[
  {
    "title": "IP Address",
    "uri": "/",
    "menu": true,
    "element": [
      {
        "name": "ipaddress",
        "type": "ACInput",
        "label": "IP Address",
        "pattern": "^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$"
      },
      {
        "name": "send",
        "type": "ACSubmit",
        "value": "SEND",
        "uri": "/check"
      }
    ]
  },
  {
    "title": "IP Address",
    "uri": "/check",
    "menu": false,
    "element": [
      {
        "name": "result",
        "type": "ACText"
      }
    ]
  }
]
)raw";

AutoConnect portal;

String checkIPAddress(AutoConnectAux& aux, PageArgument& args) {
  AutoConnectAux&   input_page = *portal.aux("/");
  AutoConnectInput& ipaddress = input_page["ipaddress"].as<AutoConnectInput>();
  AutoConnectText&  result = aux["result"].as<AutoConnectText>();

  if (ipaddress.isValid()) {
    result.value = "IP Address " + ipaddress.value + " is OK.";
    result.style = "";
  }
  else {
    result.value = "IP Address " + ipaddress.value + " error.";
    result.style = "color:red;";
  }
  return String("");
}

void setup() {
  portal.load(input_page);
  portal.on("/check", checkIPAddress);
  portal.begin();
}

void loop() {
  portal.handleClient();
}
