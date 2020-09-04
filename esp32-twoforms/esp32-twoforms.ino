
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
WebServer server;
AutoConnect portal(server);
AutoConnectAux Input;
#define INPUT_URI   "/input"
const static char InputPage[] PROGMEM = R"r(
{
  "title": "Input", "uri": "/", "menu": true, "element": [
    { "name": "input", "type": "ACInput", "label": "INPUT" },
    {
      "name": "save",
      "type": "ACSubmit",
      "value": "SAVE",
      "uri": "/input"
    }
  ]
}
)r";

// An on-page handler for '/' access
void onRoot() {
  String  content =
  "<html>"
  "<head><meta name='viewport' content='width=device-width, initial-scale=1'></head>"
  "<body><div>INPUT: {{value}}</div></body>"
  "</html>";

  Input.fetchElement();    // Preliminary acquisition

  // For this steps to work, need to call fetchElement function beforehand.
  String value = Input["input"].value;

  Serial.println(value);
  WebServer&  server = portal.host();
  //content.replace("{{value}}", value);
  //server.send(200, "text/html", content);
    server.sendHeader("Location", String("http://") + server.client().localIP().toString() + String("/"));
  server.send(302, "text/plain", "");
  server.client().flush();
  server.client().stop();
}

void setup() {
    delay(1000);
  Serial.begin(115200);
  Serial.println();
  WiFi.begin("WG21", "Pakistan1");//Sajjad Debugging
  Input.load(InputPage);
  portal.join(Input);
  server.on("/input", onRoot);  // Register the on-page handler
  portal.begin();  
}

void loop() {
  portal.handleClient();
}
