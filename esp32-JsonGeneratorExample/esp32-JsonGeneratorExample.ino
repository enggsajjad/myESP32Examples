// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2019
// MIT License
//
// This example shows how to generate a JSON document with ArduinoJson.
//
// https://arduinojson.org/v6/example/generator/

#include <ArduinoJson.h>

void setup() {
  // Initialize Serial port
      char jsonbuffer[2048];
    size_t j = sizeof(jsonbuffer) / sizeof(jsonbuffer[0]);

  Serial.begin(115200);
  while (!Serial) continue;

  // Allocate the JSON document
  //
  // Inside the brackets, 200 is the RAM allocated to this document.
  // Don't forget to change this value to match your requirement.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<200> doc;

  // StaticJsonObject allocates memory on the stack, it can be
  // replaced by DynamicJsonDocument which allocates in the heap.
  //
  // DynamicJsonDocument  doc(200);

  // Add values in the document
  //
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;

  // Add an array.
  //
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  // Generate the minified JSON and send it to the Serial port.
  //
  serializeJson(doc, Serial);
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  // Start a new line
  Serial.println();

  // Generate the prettified JSON and send it to the Serial port.
  //
  serializeJsonPretty(doc, Serial);
  // The above line prints:
  // {
  //   "sensor": "gps",
  //   "time": 1351824120,
  //   "data": [
  //     48.756080,
  //     2.302038
  //   ]
  // }
  StaticJsonDocument<512> doc1;
  JsonObject root = doc1.to<JsonObject>(); //root object filled with further json obejcts

  root["name"] = "Nova SDS011";
  root["description"] = "A low-cost sensor measuring particulate matter";
  root["encodingType"] = "application/pdf";
  root["metadata"] = "https://www.watterott.com/media/files_public/fwyjbmbnj/SDS011.pdf";
  root["@iot.id"] = "saqn:s:b3dd2c9";
  Serial.println("j");
  Serial.println(j);
  serializeJson(doc1, jsonbuffer, j);
  Serial.println("jsonbuffer");
  Serial.println(jsonbuffer);
  for(int i=0;i<285;i++)
  {
    Serial.print(i+1);Serial.print(" ");Serial.println(jsonbuffer[i]);
    }

  ////////////
  String jsonString;
  serializeJson(doc1, jsonString);
  Serial.println("jsonString");
  Serial.println(jsonString);
}
void loop() {
  // not used in this example
}

// See also
// --------
//
// https://arduinojson.org/ contains the documentation for all the functions
// used above. It also includes an FAQ that will help you solve any
// serialization problem.
//
// The book "Mastering ArduinoJson" contains a tutorial on serialization.
// It begins with a simple example, like the one above, and then adds more
// features like serializing directly to a file or an HTTP request.
// Learn more at https://arduinojson.org/book/
// Use the coupon code TWENTY for a 20% discount ❤❤❤❤❤
