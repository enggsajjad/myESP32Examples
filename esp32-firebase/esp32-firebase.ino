#include "FirebaseESP32.h"
#include <WiFi.h>
#include <Adafruit_NeoPixel.h>
#define PIN 5
#define NUM_LEDS 8
const char* ssid = "WG21";
const char* password = "Pakistan1";
FirebaseData firebaseData;
Adafruit_NeoPixel leds(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
// Current color values
int redValue = 0;
int greenValue = 0;
int blueValue = 0;
void setup() {
  delay(100);
  Serial.begin(115200);
  connectWifi();
  leds.begin();
  Serial.println("LED Begin");
  //Firebase.begin("https://xxxx.firebaseio.com/", "wDsHB30jVN554CA********");
  Firebase.begin("testled-d460d.firebaseio.com", "qi3jkdc99ZJyyOrLqsR2mdy2ZkaiezQpfVCTfFM8");
  Serial.println("Firebase Begin");
}
void loop() {
  if (Firebase.getInt(firebaseData, "/red")) {
    //Serial.println("redValue: 1");
    if  (firebaseData.dataType() == "int") {
      //Serial.println("redValue: 2");
      int val = firebaseData.intData();
      Serial.println("val: "+ String(val));
      if (val != redValue) {
        redValue = val;
        Serial.println("redValue: "+ String(redValue));
         setLedColor();
      }
    }
  }
  if (Firebase.getInt(firebaseData, "/green")) {
    if  (firebaseData.dataType() == "int") {
      int val = firebaseData.intData();
      if (val != greenValue) {
        greenValue = val;
        Serial.println("greenValue: "+ String(greenValue));
        setLedColor();
      }
    }
  }
  if (Firebase.getInt(firebaseData, "/blue")) {
    if  (firebaseData.dataType() == "int") {
      int val = firebaseData.intData();
      if (val != blueValue) {
        blueValue = val;
        Serial.println("blueValue: "+ String(blueValue));
        setLedColor();
      }
    }
  }
}
void connectWifi() {
  // Let us connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".......");
  Serial.println("WiFi Connected....IP Address:");
  Serial.println(WiFi.localIP());
}
void setLedColor() {
  for (int i=0; i < NUM_LEDS; i++) 
    leds.setPixelColor(i, leds.Color(redValue, greenValue, blueValue));
   leds.show();
}
