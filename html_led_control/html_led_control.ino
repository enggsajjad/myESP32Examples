/*
 * Sketch: ESP8266_LED_CONTROL_AJAX_01
 * Intended to be run on an ESP8266
 */
 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <meta charset='utf-8'>
 <style>
  body {font-size:140%;} 
  #main {display: table; margin: auto;  padding: 0 10px 0 10px; } 
  h2 {text-align:center; } 
  .button { padding:10px 10px 10px 10px; width:100%;  background-color: #50FF50; font-size: 120%;}
 </style>
 
 <title>LED Control</title>
</head>
<body>
 <div id='main'>
  <h2>LED Control</h2>
)=====";
 
String html_2 = "";
 
String html_4 = R"=====(
  </div>
 </body>
</html>
)=====";
 
 
#include <WiFi.h>
 
// change these values to match your network
char ssid[] = "WG21";       //  your network SSID (name)
char pass[] = "Pakistan1";          //  your network password
 
WiFiServer server(80);
 
String request = "";
//int LED_Pin = D1;
 
 
void setup() 
{
    pinMode(LED_BUILTIN, OUTPUT);     
 
    Serial.begin(115200);
    Serial.println();
    Serial.println("Serial started at 115200");
    Serial.println("ESP8266_LED_CONTROL_AJAX_01");
    Serial.println();
 
    // Connecting to a WiFi network
    Serial.print(F("Connecting to "));  Serial.println(ssid);
    WiFi.begin(ssid, pass);
 
    while (WiFi.status() != WL_CONNECTED) 
    {
        Serial.print(".");
        delay(500);
    }
 
    Serial.println("");
    Serial.println(F("[CONNECTED]"));
    Serial.print("[IP ");              
    Serial.print(WiFi.localIP()); 
    Serial.println("]");
 
    // start a server
    server.begin();
    Serial.println("Server started");
 
} // void setup()
 
 
 
void loop() 
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    Serial.print("request: "); Serial.println(request); 
 
    if       ( request.indexOf("LEDON") > 0 )  { digitalWrite(LED_BUILTIN, HIGH);  }
    else if  ( request.indexOf("LEDOFF") > 0 ) { digitalWrite(LED_BUILTIN, LOW);   }
 
 
    // Get the LED pin status and create the LED status message
    if (digitalRead(LED_BUILTIN) == HIGH) 
    {
        // the LED is on so the button needs to say turn it off
       html_2 = "  <form id='F1' action='LEDOFF'><input class='button' type='submit' value='Turn of the LED' ></form><br>\r\n";
    }
    else                              
    {
        // the LED is off so the button needs to say turn it on
        html_2 = "  <form id='F1' action='LEDON'><input class='button' type='submit' value='Turn on the LED' ></form><br>\r\n";
    }
 
 
    client.flush();
 
    client.print( header );
    client.print( html_1 );    
    client.print( html_2 );
    client.print( html_4);
 
    delay(5);
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
 
} // void loop()
