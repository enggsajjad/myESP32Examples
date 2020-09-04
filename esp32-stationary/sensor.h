



/*
    Nova SDS011
*/

HardwareSerial SDS_SERIAL(2); // SDS011: use Serial 2 
// connect TXD of Dust Sensor SDS011 to Pin 13 of ESP32 (ESP-RX). 
// connect 5V of Dust Sensor SDS011 to Pin 5V of ESP32. 
// connect GND of Dust Sensor SDS011 to Pin GND of ESP32.

SDS011 sds; // sensor object



/*
   BME280
*/

Adafruit_BME280 bme; 
// connect SCK of BME280 sensor to Pin 22 of ESP32 (ESP-SCL)
// connect SDA of BME280 sensor to Pin 21 of ESP32 (ESP-SDA)
// connect VCC of BME280 sensor to 3V3 of ESP32
// connect GND of BME280 sensor to GND of ESP32


/*
   DHT22
*/

DHT dht(21, DHT22);
#ifdef DHT22 // conflicts of same defining by dht lib and esp32_thingsapi lib
#undef DHT22
#endif 
// look at the sensor from the front (grid shows in your direction), put the pins facing down
// start counting the pins from left (pin1) to right (pin4). pin 3 is not connected
// Connect pin 1 of DHT22 sensor (on the left) to 3V3 of ESP32
// Connect pin 2 of DHT22 sensor to Pin 21 of ESP32 (data)
// Connect pin 4 of DHT22 sensor (on the right) to GND of ESP32
// Connect a resistor in range 4.7k to 10k from pin 2 (data) to pin 1 (power) of the sensor
