#include "FirebaseESP32.h"
#include <WiFi.h>
/* Controlling LED using Firebase console by CircuitDigest(www.circuitdigest.com) */
//#include <FirebaseArduino.h>                                             // firebase library

//#define FIREBASE_HOST "dummy-d6efb.firebaseio.com"                         // the project name address from firebase id
//#define FIREBASE_AUTH "LqkiyF4SskAKgKDQFZBoe23vc2r8BfdBTzWXAlBB"                    // the secret key generated from firebase
#define FIREBASE_HOST "smartkit-startr.firebaseio.com"
#define FIREBASE_AUTH "JcYg8QyG2iuMVju9OKPaCarAvMxrayfpsDujSaf8"

#define WIFI_SSID "WG21"                                          // input your home or public wifi name 
#define WIFI_PASSWORD "Pakistan1"                                    //password of wifi ssid
FirebaseData firebaseData;
String fireStatus = "";                                                     // led status received from firebase
int LED13 = LED_BUILTIN;                                                                // for external led
int Signal;                // holds the incoming raw data. Signal value can range from 0-1024
int Threshold = 550;            // Determine which Signal to "count as a beat", and which to ingore.


void setup(void)
{
  Serial.begin(115200); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("WIFI not connecting...");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Serial.println(FIREBASE_HOST);
  pinMode(LED13,OUTPUT);
  //sensors.begin();
  //dht.begin();
}
String userType;
String currentKey;

void loop(void)
{ 
  //Fetch the current activity (only for single patient or doctor)
  // for more than one patient or doctor you have to add another variable to count these entities.
  if (Firebase.getString(firebaseData, "/stack/userType")) 
  {
    if  (firebaseData.dataType() == "string") 
    {
      userType = firebaseData.stringData();
      Serial.println("userType: "+ (userType));
      
    }
  }

  //differentiat between doctor and patient
  if (userType == "doctor")
  {
    //fetch current doctor
    if (Firebase.getString(firebaseData, "/stack/curDoctor")) 
    {
      if  (firebaseData.dataType() == "string") 
      {
        currentKey = firebaseData.stringData();
        Serial.println("currentKey of Doctor: "+ (currentKey));
        
      }
    }
  }
  else if (userType == "patient")
  {
    //fetch current patient
    if (Firebase.getString(firebaseData, "/stack/curPatient")) 
    { 
      if  (firebaseData.dataType() == "string") 
      {
        currentKey = firebaseData.stringData();
        Serial.println("currentKey of Patient: "+ (currentKey));
        
      }
    }    
  }
  //now you have current patient or doctor key in the variable currentKey
  
  delay(30000);
  //sensors.requestTemperatures();                // Send the command to get temperatures  
  Serial.println("Celsius Temperature: ");
  float celsTemp = 23.56;//sensors.getTempCByIndex(0);
  Serial.println(celsTemp); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire

  float celsTemp1;

  if(celsTemp < 0)
  {
    celsTemp1 = 0;
  }
  else
  {
   celsTemp1 = celsTemp;
  }

  String celsiusTemp = String(celsTemp1)+String("Â°C");
  
  Serial.println("Fahrenheit temperature: ");
  float farhTemp = 434.23;//sensors.getTempFByIndex(0);  
  Serial.println(farhTemp);
  float farhTemp1;

  if(farhTemp < 0)
  {
    farhTemp1 = 0;
  }
  else
  {
   farhTemp1 = farhTemp;
  }

  
  String farhientTemp = String(farhTemp1)+String("Â°F");

  Firebase.setInt(firebaseData,"/bodytemp1",celsTemp1);
  Firebase.setInt(firebaseData,"/bodytemp2",farhTemp1);

  //Firebase.pushString(firebaseData,"/patient/7Njc7GpJD2WXqoR5vQEjvQhco2b2/sensorsData/bodytemperature",farhientTemp);
  //Dynamically update the content based on currentKey and userType
  Firebase.pushString(firebaseData,"/"+userType+"/"+currentKey+"/sensorsData/bodytemperature",farhientTemp);

  float h = 11.11;//dht.readHumidity();
  float t = 22.22;//dht.readTemperature();  

  if(isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read DHT Sensor!"));
    return;
    }

   Serial.print("Humidity"); Serial.print(h);
   String firehumid = String(h)+String("%");

   Serial.print("% Temperature: "); Serial.print(t); Serial.println("Â°C");
   String fireTemp = String(t)+String("Â°C");

   Firebase.setInt(firebaseData,"/Humidity",h);
   Firebase.setInt(firebaseData,"/RoomTemperature",t);

   //Dynamically update the content based on currentKey and userType
   Firebase.pushString(firebaseData,"/"+userType+"/"+currentKey+"/sensorsData/humidity",firehumid);
   Firebase.pushString(firebaseData,"/"+userType+"/"+currentKey+"/sensorsData/roomtemperature",fireTemp);

  Signal = 323;//analogRead(PulseSensorPurplePin); 
  int signal1 = Signal/6;  
  int signal2 = (Signal/6)-5;// Read the PulseSensor's value.
                                              // Assign this value to the "Signal" variable.

  Serial.println(signal1);   
  Serial.println(signal2);// Send the Signal value to Serial Plotter.


   if(Signal > Threshold){                          // If the signal is above "550", then "turn-on" Arduino's on-Board LED.
     digitalWrite(LED13,HIGH);
   } else {
     digitalWrite(LED13,LOW);                //  Else, the sigal must be below "550", so "turn-off" this LED.
   }
   String firebeat = String(signal1)+String("bpm");
   String firepulse = String(signal2)+String("ppm");

   Firebase.setInt(firebaseData,"/Heartbeat",signal1);
   Firebase.setInt(firebaseData,"/PulseRate",signal2);

   //Firebase.pushString("/SensorsData/Heartbeat",firebeat);
   //Dynamically update the content based on currentKey and userType
   Firebase.pushString(firebaseData,"/"+userType+"/"+currentKey+"/sensorsData/pulserate",firepulse);

   
}
