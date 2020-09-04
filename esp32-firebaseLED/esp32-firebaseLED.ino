#include "FirebaseESP32.h"
#include <WiFi.h>
/* Controlling LED using Firebase console by CircuitDigest(www.circuitdigest.com) */
//#include <FirebaseArduino.h>                                             // firebase library

#define FIREBASE_HOST "dummy-d6efb.firebaseio.com"                         // the project name address from firebase id
#define FIREBASE_AUTH "LqkiyF4SskAKgKDQFZBoe23vc2r8BfdBTzWXAlBB"                    // the secret key generated from firebase
#define WIFI_SSID "WG21"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "Pakistan1"                                    //password of wifi ssid
FirebaseData firebaseData;
String fireStatus = "";                                                     // led status received from firebase
int led = LED_BUILTIN;                                                                // for external led
void setup() {
  delay(100);
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);      
  pinMode(led, OUTPUT);                 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                      //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());                                                      //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                                       // connect to firebase
  Firebase.setString(firebaseData, "/LED_STATUS", "ON");                                          //send initial string of led status
}

void loop() {
  //fireStatus = Firebase.getString(firebaseData, "LED_STATUS");                                      // get ld status input from firebase
  if (Firebase.getString(firebaseData, "/LED_STATUS")) 
  {
    if  (firebaseData.dataType() == "string") 
    {
      fireStatus = firebaseData.stringData();
      Serial.println("fireStatus: "+ (fireStatus));
      
    }
  }
  delay(2000);
  if (fireStatus == "ON") 
  {                                                          // compare the input of led status received from firebase
    Serial.println("Led Turned ON");                         
    digitalWrite(LED_BUILTIN, LOW);                                                  // make bultin led ON
    digitalWrite(led, HIGH);                                                         // make external led ON
  } 
  else if (fireStatus == "OFF") 
  {                                                  // compare the input of led status received from firebase
    Serial.println("Led Turned OFF");
    digitalWrite(LED_BUILTIN, HIGH);                                               // make bultin led OFF
    digitalWrite(led, LOW);                                                         // make external led OFF
  }
  else 
  {
    //Serial.println("Wrong Credential! Please send ON/OFF");
  }
}
