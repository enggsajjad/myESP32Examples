extern bool use_bme280;
extern bool use_dht22;
#define SOFTWARE_VERSION "esp32-crowdsensor-v0.8";


String json_madavi(bool status_sds, float pm10, float pm25, float temp, float hum, float atm) {
  // create Json buffer for send2madavi-data. inside brackets: size of the pool in bytes.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<1000> jsonBuffer; //TODO set a good buffer size
  // create json object
  JsonObject& data_json = jsonBuffer.createObject();
  // add all key-value pairs to json object
  data_json["esp32id"] = esp32_chipid;
  data_json["software_version"] = SOFTWARE_VERSION;

  JsonArray& sensordatavalues = data_json.createNestedArray("sensordatavalues");

  if (!status_sds) {
    JsonObject& sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "SDS_P1";
    sensordatavalues_0["value"] = pm10;
    JsonObject& sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "SDS_P2";
    sensordatavalues_1["value"] = pm25;
  }

  if (use_dht22 && !isnan(temp)) {
    JsonObject& sensordatavalues_2 = sensordatavalues.createNestedObject();
    sensordatavalues_2["value_type"] = "temperature";
    sensordatavalues_2["value"] = temp;
    JsonObject& sensordatavalues_3 = sensordatavalues.createNestedObject();
    sensordatavalues_3["value_type"] = "humidity";
    sensordatavalues_3["value"] = hum;
  }

  if (use_bme280 && !isnan(temp)) {
    JsonObject& sensordatavalues_4 = sensordatavalues.createNestedObject();
    sensordatavalues_4["value_type"] = "BME280_temperature";
    sensordatavalues_4["value"] = temp;
    JsonObject& sensordatavalues_5 = sensordatavalues.createNestedObject();
    sensordatavalues_5["value_type"] = "BME280_humidity";
    sensordatavalues_5["value"] = hum;
    JsonObject& sensordatavalues_6 = sensordatavalues.createNestedObject();
    sensordatavalues_6["value_type"] = "BME280_pressure";
    sensordatavalues_6["value"] = atm;
  }

  String output;
  data_json.printTo(output);
  return output;
}


String json_sds(bool status_sds, float pm10, float pm25) {
  // create Json buffer for send2madavi-data. inside brackets: size of the pool in bytes.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<1000> jsonBuffer; //TODO set a good buffer size
  // create json object
  JsonObject& data_json = jsonBuffer.createObject();
  // add all key-value pairs to json object
  data_json["esp32id"] = esp32_chipid;
  data_json["software_version"] = SOFTWARE_VERSION;

  JsonArray& sensordatavalues = data_json.createNestedArray("sensordatavalues");

  if (!status_sds) {
    JsonObject& sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "P1";
    sensordatavalues_0["value"] = pm10;
    JsonObject& sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "P2";
    sensordatavalues_1["value"] = pm25;
  }

  String output;
  data_json.printTo(output);
  return output;
}


String json_RHT(float temp, float hum, float atm) {
  // create Json buffer for send2madavi-data. inside brackets: size of the pool in bytes.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonBuffer<1000> jsonBuffer; //TODO set a good buffer size
  // create json object
  JsonObject& data_json = jsonBuffer.createObject();
  // add all key-value pairs to json object
  data_json["esp32id"] = esp32_chipid;
  data_json["software_version"] = SOFTWARE_VERSION;

  JsonArray& sensordatavalues = data_json.createNestedArray("sensordatavalues");


  if (use_bme280 && !isnan(temp)) {
    JsonObject& sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "BME280_temperature";
    sensordatavalues_0["value"] = temp;
    JsonObject& sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "BME280_humidity";
    sensordatavalues_1["value"] = hum;
    JsonObject& sensordatavalues_2 = sensordatavalues.createNestedObject();
    sensordatavalues_2["value_type"] = "BME280_pressure";
    sensordatavalues_2["value"] = atm;
  }

  else if (use_dht22 && !isnan(temp)) {
    JsonObject& sensordatavalues_0 = sensordatavalues.createNestedObject();
    sensordatavalues_0["value_type"] = "temperature";
    sensordatavalues_0["value"] = temp;
    JsonObject& sensordatavalues_1 = sensordatavalues.createNestedObject();
    sensordatavalues_1["value_type"] = "humidity";
    sensordatavalues_1["value"] = hum;
  }

  String output;
  data_json.printTo(output);
  return output;
}
