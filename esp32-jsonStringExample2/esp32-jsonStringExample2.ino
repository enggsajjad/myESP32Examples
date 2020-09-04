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
  
  String s1 =  "2019-12-31T02:29:54.000Z";
  String s2 =  "2019-12-31T01:29:55.000Z";
  String s3 =  "2019-12-31T01:29:53.000Z";
  if (s1>s2) Serial.println("S1 is Greater than S2");
  else  Serial.println("S1 is less than S2");
  
  if (s1>s3) Serial.println("S1 is Greater than S3");
  else  Serial.println("S1 is less than S3");

  String v1 =  "0.8.7.0";
  String v2 =  "0.8.8.0";
  String v3 =  "0.6.9.0";
  if (v1>v2) Serial.println("v1 is Greater than v2");
  else  Serial.println("v1 is less than v2");
  
  if (v1>v3) Serial.println("v1 is Greater than v3");
  else  Serial.println("v1 is less than v3");

  String swTime[3];
  String swVersion[3];
  swTime[0] = "2018-12-24T10:13:36.313Z";
  swVersion[0] = "v0.8.5";
  swTime[1] = "2018-12-27T11:02:54.104Z";
  swVersion[1] = "v0.8.6";
  swTime[2] = "2018-12-27T13:52:14.523Z";
  swVersion[2] = "v0.8.5";

  String t1 = "2018-12-24T10:13:36.313Zv0.8.5";
  String t2 = "2018-12-27T11:02:54.104Zv0.8.6";
  String t3 = "2018-12-27T13:52:14.523Zv0.8.5";

  if(t1>t2) Serial.println("t1 is Greater than t2");
  else  Serial.println("t1 is less than t2");

  if(t1>t3) Serial.println("t1 is Greater than t3");
  else  Serial.println("t1 is less than t3");

  if(t2>t3) Serial.println("t2 is Greater than t3");
  else  Serial.println("t2 is less than t3");

  DynamicJsonDocument doc(1024);
  float d[2];
  // You can use a String as your JSON input.
  // WARNING: the string in the input  will be duplicated in the JsonDocument.
  String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  Serial.println(input);
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

 
  // You can use a String to get an element of a JsonObject
  // No duplication is done.
  long time = obj[String("time")];
  Serial.println("Time "+String(time));
  // You can use a String to set an element of a JsonObject
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  obj[String("time")] = time+10;

  // You can get a String from a JsonObject or JsonArray:
  // No duplication is done, at least not in the JsonDocument.
  String sensor = obj["sensor"];

  Serial.println("sensor "+String(sensor));
  
  // Unfortunately, the following doesn't work (issue #118):
  // sensor = obj["sensor"]; // <-  error "ambiguous overload for 'operator='"
  // As a workaround, you need to replace by:
  sensor = obj["sensor"].as<String>();

  // You can set a String to a JsonObject or JsonArray:
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  obj["sensor"] = sensor+" test";

  // It works with serialized() too:
  //obj["sensor"] = serialized(sensor);

  // You can also concatenate strings
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  //obj[String("sen") + "sor"] = String("gp") + "s";

  // You can compare the content of a JsonObject with a String
  if (obj["sensor"] == sensor) {
    // ...
  }

  // Lastly, you can print the resulting JSON to a String
  String output;
  serializeJson(doc, output);
  Serial.println(output);

///////////////////////////////////////////
  char JSONMessage[] = " {\"SensorType\": \"Temperature\", \"Value\": [20, 21, 23]}";
  Serial.print("Message to parse: ");
  Serial.println(JSONMessage);
 
  //StaticJsonBuffer<300> JSONBuffer; //Memory pool
  StaticJsonDocument<200> JSONBuffer;
  //JsonObject& parsed = JSONBuffer.parseObject(JSONMessage);   //Parse message
  deserializeJson(JSONBuffer, JSONMessage);
 
  
 
  const char * sensorType = JSONBuffer["SensorType"]; //Get sensor type value
  Serial.print("Sensor type: ");
  Serial.println(sensorType);
 
  int arraySize = JSONBuffer["Value"].size();   //get size of JSON Array
  Serial.print("\nSize of value array: ");
  Serial.println(arraySize);
 
  Serial.println("\nArray values without explicit casting");
  for (int i = 0; i < arraySize; i++) { //Iterate through results
 
    int sensorValue = JSONBuffer["Value"][i];  //Implicit cast
    Serial.println(sensorValue);
 
  }
 
  Serial.println("\nArray values with explicit casting");
  for (int i = 0; i < arraySize; i++) {  //Iterate through results
 
    Serial.println(JSONBuffer["Value"][i].as<int>());//Explicit cast
 
  }
 /////////////////////////////////////////
 arraySize = doc["data"].size();   //get size of JSON Array
   Serial.println("\nArray values with explicit casting");
  for (int i = 0; i < arraySize; i++) {  //Iterate through results
 
    Serial.println(doc["data"][i].as<float>());//Explicit cast
 
  }
  /////////////////////////////////////////
 Serial.println("\nTesting1\n-----------------------");
 Serial.println("Original Json Message:");
  //input = "{\"properties\": {\"FW_VERSION\": {\"11.07.2019\": \"intermediate1 jan 0.5\",\"10.07.2019\": \"intermediate jan 0.6\"}}}";
  input = "{\"properties\":{\"software.version\":{\"2018-12-24T10:13:36.313Z\":\"v0.8.5\",\"2018-12-27T11:02:54.104Z\":\"v0.8.6\",\"2018-12-27T13:52:14.523Z\":\"v0.8.5\"},\"hardware.revision\":{\"2019-08-01T:00:00:00.000Z\":\"https://www.teco.edu/wp-content/grand-media/application/Anleitung_Crowdsensor_V085b.pdf\"},\"hardware.id\":\"180232\",\"shortname\":\"crowdsensor\",\"operator.domain\":\"teco.edu\"}}";
  Serial.println(input);
  deserializeJson(doc, input);
  JsonObject obj1 = doc.as<JsonObject>();
  //Get the nested element: Example
  Serial.println("Example: Get the nested element: obj1[\"properties\"][\"software.version\"][\"2018-12-24T10:13:36.313Z\"]");
  String sr = obj1["properties"]["software.version"]["2018-12-24T10:13:36.313Z"];
  Serial.println("sensor "+sr);
  //Get the nested element: Example
  Serial.println("Example: Get the nested element: obj1[\"properties\"][\"software.version\"]");  
  JsonObject obj2 = obj1["properties"]["software.version"];
  String sr2 = obj2["2018-12-24T10:13:36.313Z"];
  Serial.println("sensor "+sr2);

  Serial.println("Example: Iterate through JSON Pair");
  String dt,ver;
  for (JsonPair kv : obj2) {
    Serial.println(kv.key().c_str());
    Serial.println(kv.value().as<char*>());
    dt = kv.key().c_str();
    ver = kv.value().as<char*>();
  }
  Serial.println("Example: Last JSON Pair");
  Serial.println(dt);
  Serial.println(ver);
  
  //Serial.println("Example: Change/Add JSON Pair");
  //obj2["2019-12-27T11:02:54.104Z"] = "v0.8.7";
  String thSwDate = "2018-12-27T13:52:18.523Z";
  String thSwVersion = "v0.8.4";
  if (thSwVersion == ver && thSwDate == dt ) //identical
  {
    Serial.println("Properties are indentical.");
  }
  else if (thSwVersion > ver || (thSwVersion <= ver && thSwDate > dt))
  {
    Serial.println("Properties are not indentical.");
    obj2[thSwDate] = thSwVersion;
    String output2;
    serializeJson(doc, output2);
    Serial.println(output2); 
  }
  //String output2;
  //serializeJson(doc, output2);
  //Serial.println(output2);
  Serial.println("Original Json Message:");
  Serial.println(input);
  
  ////////////////////////////////////
  Serial.println("\nTesting2\n-----------------------");
  Serial.println("Example: Change/Add JSON Pair");
  StaticJsonDocument<1024> doc1;
  JsonObject root = doc1.to<JsonObject>(); //root object filled with further json obejcts
  JsonObject propertyObject  = root.createNestedObject("properties");
  
  JsonObject firmwareObject  = propertyObject.createNestedObject("software.version");
  //slightly different structore here than usual, cause date (key) itsels is varying
  firmwareObject["2018-12-24T10:13:36.313Z"] = "v0.8.5";
  firmwareObject["2018-12-27T11:02:54.104Z"] = "v0.8.6";

  String output3;
  serializeJson(doc1, output3);
  Serial.println(output3);
  
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
