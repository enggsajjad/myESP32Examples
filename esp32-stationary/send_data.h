// send data to madavi

extern String esp32_chipid;
extern bool use_bme280;
extern bool use_dht22;

void sendData(const char *data_json, char destination[], String pin) {
  
  
  HTTPClient http;   
  
  http.begin(destination);  //Specify destination for HTTP request
  http.addHeader("Content-Type", "application/json");             //Specify content-type header
  http.addHeader("X-PIN", pin);
  http.addHeader("X-Sensor", "esp32-"+esp32_chipid);
  
  int httpResponseCode = http.POST(data_json);   //Send the actual POST request
  Serial.print("data: ");
  Serial.println(data_json);
  if(httpResponseCode>0){
  
  String response = http.getString();                       //Get the response to the request
  Serial.print("httpResponseCode: ");
  Serial.println(httpResponseCode);   //Print return code
  Serial.print("response: ");           //Print request answer
  Serial.println(response);         
//  return "hallo";
  }
  else{
  Serial.print("Error on sending POST: ");
  Serial.println(httpResponseCode);
  }
   http.end();  //Free resources
}





