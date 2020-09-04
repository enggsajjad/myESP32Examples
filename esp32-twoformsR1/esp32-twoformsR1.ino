
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
// Declare AutoConnectElements for the page as /mqtt_save
ACText(caption2, "<h4>Parameters available as:</h4>", "text-align:center;color:#2f4f4f;padding:10px;");
//ACText(parameters, "<h1>MQTT broker settings</h1>", "text-align:center;color:#2f4f4f;padding:10px;");
ACText(parameters, "<h1>MQTT broker settings</h1>", "text-align:center;color:#2f4f4f;");
//ACText(parameters3, "", "font-size:20px;text-align:center;color:#2f4f4f;padding:10px;");
ACText(parameters3, "", "text-align:center;color:#2f4f4f;");
ACInput(userkey, "", "User Key");
ACInput(parameters2, "parameters2", "Parameter Cap");
ACSubmit(save, "SAVE", "/input");
const char* scConfirmBox = R"(
<script>
function ConfirmBox() {
  if (confirm("Daten speichern?")) 
  window.location.href = "/input";
}
</script>
)";

ACButton(cfgButton, "Speichern", "ConfirmBox()");
ACElement(cfgScript2, scConfirmBox);
// Declare the custom Web page as /mqtt_save and contains the AutoConnectElements
AutoConnectAux Input2("/", "MQTT Setting", true, {
  //style,
  caption2,
  userkey,
  parameters,
  parameters3,
  parameters2,
  save,
  cfgButton,
  cfgScript2
});

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
void onRoot2() {
  String  content =
  "<html>"
  "<head><meta name='viewport' content='width=device-width, initial-scale=1'></head>"
  "<body><div>INPUT: {{value}}</div></body>"
  "</html>";

  Input2.fetchElement();    // Preliminary acquisition

  // For this steps to work, need to call fetchElement function beforehand.
  String value = Input2["userkey"].value;
  Serial.println(value);
  value = Input2["parameters2"].value;

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

  Input2["userkey"].value= "SAJJAD HUSSAIN";
  
  Input2["parameters2"].value = "Parameters";

Input2["parameters3"].value = "<h1>MQTT broker settings</h1>";
//Input2["parameters"].format= "text-align:center;color:#2f4f4f;padding:10px;"
  
  //Input.load(InputPage);
  //portal.join(Input);
  //Input2.load(Input2);
  portal.join(Input2);
  server.on("/input", onRoot2);  // Register the on-page handler
  portal.begin();  
}

void loop() {
  portal.handleClient();
}
