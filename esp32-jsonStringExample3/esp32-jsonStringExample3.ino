// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License
//
// This example shows the different ways you can use String with ArduinoJson.
//
// Use String objects sparingly, because ArduinoJson duplicates them in the
// JsonDocument. Prefer plain old char[], as they are more efficient in term of
// code size, speed, and memory usage.
//
// https://arduinojson.org/v6/example/string/

#include <ArduinoJson.h>

void setup() {
  delay(100);
  Serial.begin(115200);
  
  DynamicJsonDocument doc(1024);

 Serial.println("\nTesting1\n-----------------------");
 Serial.println("Original Json Message:");
 String input;
  //Recent TimeStamp  at the end, version not identical
  //input = "{\"properties\":{\"software.version\":{\"2018-12-24T10:13:36.000Z\":\"v0.8.5\",\"2018-12-27T11:02:54.000Z\":\"v0.8.6\",\"2018-12-27T13:52:14.000Z\":\"v0.8.5\"},\"hardware.revision\":{\"2019-08-01T:00:00:00.000Z\":\"https://www.teco.edu/wp-content/grand-media/application/Anleitung_Crowdsensor_V085b.pdf\"},\"hardware.id\":\"180232\",\"shortname\":\"crowdsensor\",\"operator.domain\":\"teco.edu\"}}";
  //Recent TimeStamp  at 2nd last, version not identical
  //input = "{\"properties\":{\"software.version\":{\"2018-12-24T10:13:36.000Z\":\"v0.8.5\",\"2018-12-27T13:52:14.000Z\":\"v0.8.6\",\"2018-12-27T11:02:54.000Z\":\"v0.8.5\"},\"hardware.revision\":{\"2019-08-01T:00:00:00.000Z\":\"https://www.teco.edu/wp-content/grand-media/application/Anleitung_Crowdsensor_V085b.pdf\"},\"hardware.id\":\"180232\",\"shortname\":\"crowdsensor\",\"operator.domain\":\"teco.edu\"}}";
  //Recent TimeStamp  at the end, version not identical
  input = "{\"properties\":{\"software.version\":{\"2018-12-27T13:52:14.000Z\":\"v0.8.5\",\"2018-12-27T11:02:54.000Z\":\"v0.8.6\",\"2018-12-24T10:13:36.000Z\":\"v0.8.5\"},\"hardware.revision\":{\"2019-08-01T:00:00:00.000Z\":\"https://www.teco.edu/wp-content/grand-media/application/Anleitung_Crowdsensor_V085b.pdf\"},\"hardware.id\":\"180232\",\"shortname\":\"crowdsensor\",\"operator.domain\":\"teco.edu\"}}";
  
  Serial.println(input);
  deserializeJson(doc, input);
  JsonObject obj1 = doc.as<JsonObject>();
  //Get the nested element: Example
  //Serial.println("Example: Get the nested element: obj1[\"properties\"][\"software.version\"][\"2018-12-24T10:13:36.313Z\"]");
  //String sr = obj1["properties"]["software.version"]["2018-12-24T10:13:36.313Z"];
  //Serial.println("sensor "+sr);
  //Get the nested element: Example
  //Serial.println("Example: Get the nested element: obj1[\"properties\"][\"software.version\"]");  
  JsonObject obj2 = obj1["properties"]["software.version"];
  //String sr2 = obj2["2018-12-24T10:13:36.313Z"];
  //Serial.println("sensor "+sr2);

  Serial.println("Example: Iterate through JSON Pair");
  String dt,ver, preDt, recentDt, recentVer;
  for (JsonPair kv : obj2) {
    Serial.println(kv.key().c_str());
    Serial.println(kv.value().as<char*>());
    dt = kv.key().c_str();
    ver = kv.value().as<char*>();

    if(dt > preDt)
    {
      recentDt = dt;
      recentVer = ver;
    } 
    preDt = dt;
  }
  Serial.println("Example: Recent JSON Pair");
  Serial.println(recentDt);
  Serial.println(recentVer);
  
  String thSwDate = "2018-12-27T13:52:18.523Z";
  String thSwVersion = "v0.8.5";
  if (thSwVersion == recentVer) //identical
  {
    Serial.println("Properties Version are indentical.");
  }
  else
  {
    Serial.println("Properties are not indentical.");
    obj2[thSwDate] = thSwVersion;
    String output2;
    serializeJson(doc, output2);
    Serial.println(output2); 
  }

  Serial.println("Original Json Message:");
  Serial.println(input);
  
}

void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any problem.
//
// The book "Mastering ArduinoJson" contains a quick C++ course that explains
// how your microcontroller stores strings in memory. On several occasions, it
// shows how you can avoid String in your program.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
