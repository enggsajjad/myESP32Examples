#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h>
 
void setup() {
 delay(500);
  Serial.begin(115200);                            //Serial connection
  WiFi.begin("WG21", "Pakistan1");   //WiFi connection
 
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");
 
  }
 delay(1000);
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
 
  StaticJsonDocument<500> doc;
/*{
"sensorType": "Temperature",
"values": [20, 21, 23],
"timestamps": ["10:10", "10:20", "10:30"]
 }
*/
doc["sensorType"] = "Temperature";

    JsonArray analogValues = doc.createNestedArray("value");
    analogValues.add(20);
    analogValues.add(21);
    analogValues.add(23);

    JsonArray digitalValues = doc.createNestedArray("timestamps");
    digitalValues.add("10:10");
    digitalValues.add("10:22");
    digitalValues.add("10:33");
    
    char JSONmessageBuffer[300];
    serializeJsonPretty(doc,JSONmessageBuffer, sizeof(JSONmessageBuffer));
    serializeJson(doc,JSONmessageBuffer, sizeof(JSONmessageBuffer));
    //char JSONmessageBuffer[300];
    //JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println(JSONmessageBuffer);

    //////////////////////////////////////////////////////////////////////////////////////////////
    HTTPClient http;    //Declare object of class HTTPClient
    Serial.println("http://sajjadhussain.pythonanywhere.com/postjson");
    http.begin("http://sajjadhussain.pythonanywhere.com/postjson");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    //String payload = http.getString();                                        //Get the response payload
    
    Serial.println(httpCode);   //Print HTTP return code
    //Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
    //////////////////////////////////////////////////////////////////////////////////////////////
  
  Serial.print("[API] URL: ");
  Serial.println("http://sajjadhussain.pythonanywhere.com/postjson");
  
  http.begin("http://sajjadhussain.pythonanywhere.com/postjson");
    
    httpCode = http.GET();
      
    String response = http.getString();
    if(httpCode < 0) {
    Serial.println("[API] Error on HTTP get");
    Serial.println("[API] HTTP Code "+String(httpCode));

    }
    else {
    Serial.println("[API] HTTP Code: "+String(httpCode));
    Serial.println("[API] response: " + response);
    }
    http.end();
    //////////////////////////////////////////////////////////////////////////////////////////////

    
    Serial.println("http://sajjadhussain.pythonanywhere.com/postsajjad");
    http.begin("http://sajjadhussain.pythonanywhere.com/postsajjad");      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    httpCode = http.POST(JSONmessageBuffer);   //Send the request
    //String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    //Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  } else {
 
    Serial.println("Error in WiFi connection");
 
  }
 
  delay(30000);  //Send a request every 30 seconds
 
}
