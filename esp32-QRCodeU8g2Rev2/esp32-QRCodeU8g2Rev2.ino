/********************************************************************************
    QRCode Generate and Display on OLED or TFT display
    Generate_Display_QRcode.ino
    2019 @phillipjacobsen
    Program Features:
    This program has been tested with ESP32 Adafruit Huzzah however it should also work with ESP8266 modules with minor changes to hardward connections and wifi libraries.
    Encodes text string into a matrix representing the QR Code
      -program does not check length of text to make sure the QRcode version is able to generate it
    Displays the resulting code on a 128x64 OLED display or a 240x320 TFT display
    The QRcode can be doubled in size so that each QR code pixel shows up as four pixels on the OLED display.
    Default is lit background with Black QRcode. Inverse can also be displayed however codes seem to scan much better with lit background.
********************************************************************************/


/********************************************************************************
               Electronic Hardware Requirements and Pin Connections
   ESP32 Adafruit Huzzah
      Source:  https://www.adafruit.com/product/3405
  Use these connections if using the 0.96 I2C display
    0.96 I2C 128x64 OLED display
      Source:    https://www.aliexpress.com/store/product/Free-shipping-Matrix-360-NAND-Programmer-MTX-USB-SPI-Flasher-V1-0-For-XBOX-360-Game/334970_1735255916.html?spm=2114.12010612.0.0.73fe44c9mSwirS
      SDA -> SDA, pin 23
      SCL -> SCL, pin 22
      VCC -> 3.3V
      GND -> GND
  Use these connections if using the TFT FeatherWing 2.4" 240x320 Touchscreen
    The TFT FeatherWing module from Adafruit is designed with headers to directly mount on the ESP32 Adafruit Huzzah.
    No additional wiring is required for operation.
      Source:   https://www.adafruit.com/product/3315
    TFT_CS  -> pin 15
    TFT_DC  -> pin 33
    RT      -> pin 32
    SD      -> pin 14
    SCK     -> SCK, pin 5
    MISO    -> MISO, pin 19
    MOSI    -> MOSI, pin 18
  Use these connections if using the TFT FeatherWing 3.5" 320x480 Touchscreen
    The TFT FeatherWing module from Adafruit is designed with headers to directly mount on the ESP32 Adafruit Huzzah.
    No additional wiring is required for operation.
    Requires: Adafruit HX8357 TFT Library,  Adafruit GFX Library
      Source:   https://www.adafruit.com/product/3651
    TFT_CS  -> pin 15
    TFT_DC  -> pin 33
    RT      -> pin 32
    SD      -> pin 14
    SCK     -> SCK, pin 5
    MISO    -> MISO, pin 19
    MOSI    -> MOSI, pin 18
  Other I/O
    LED on Huzzah board -> pin 13
********************************************************************************/


/********************************************************************************
  Conditional Assembly
********************************************************************************/
//select one of the following 2 screens
#define _128x64_OLED      // Uncomment this if you are using the 0.96 OLED display


/********************************************************************************
                              Library Requirements
********************************************************************************/

/********************************************************************************
    QRCode by Richard Moore version 0.0.1
      Available through Arduino Library Manager
        https://github.com/ricmoo/QRCode
    The QR code data encoding algorithm defines a number of 'versions' that increase in size and store increasing amounts of data.
    The version (size) of a generated QR code depends on the amount of data to be encoded.
    Increasing the error correction level will decrease the storage capacity due to redundancy pixels being added.
    If you have a '? character in your QR text then I think the QR code operates in "Byte" mode.
********************************************************************************/
//#include "QRCode/src/qrcode.h"
#include "qrcode.h"
QRCode qrcode;                  // Create the QR code

#ifdef _128x64_OLED
//version 3 code with double sized code and starting at y0 = 2 is good
//version 3 with ECC_LOW gives 53 "bytes".
const int QRcode_Version = 3;   //  set the version (range 1->40)
const int QRcode_ECC = 0;       //  set the Error Correction level (range 0-3) or symbolic (ECC_LOW, ECC_MEDIUM, ECC_QUARTILE and ECC_HIGH)
#define _QR_doubleSize    //
#define Lcd_X  128
#define Lcd_Y  64

#endif




#ifdef _128x64_OLED
/********************************************************************************
  U8g2lib Monochrome Graphics Display Library
  This library is used for the 0.96" OLED
    Available through Arduino Library Manager
    https://github.com/olikraus/u8g2
  Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
  Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
  U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
********************************************************************************/
#include <U8g2lib.h>
#include <U8x8lib.h>

// U8g2 Constructor List for Frame Buffer Mode.
// This uses the Hardware I2C peripheral on ESP32 with DMA interface
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ 15, /* data=*/ 4);
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
#endif

void setPixelsHigh(){
  for (uint8_t y = 0; y < 63; y++) {
    for (uint8_t x = 0; x < 127; x++) {
      u8g2.setDrawColor(1);       //change to 0 to make QR code with black background
      u8g2.drawPixel(x, y);
    }
  }
}
void printQRCode()
{
    //    This prints the QR code to the serial monitor as solid blocks. Each module
  //    is two characters wide, since the monospace font used in the serial monitor
  //    is approximately twice as tall as wide.
  // Top quiet zone
  Serial.print("\n\n\n\n");
  for (uint8_t y = 0; y < qrcode.size; y++) {
    // Left quiet zone
    Serial.print("        ");
    // Each horizontal module
    for (uint8_t x = 0; x < qrcode.size; x++) {
      // Print each module (UTF-8 \u2588 is a solid block)
      Serial.print(qrcode_getModule(&qrcode, x, y) ? "\u2588\u2588" : "  ");
    }
    Serial.print("\n");
  }
  // Bottom quiet zone
  Serial.print("\n\n\n\n");


}
 void displayQRCode(unsigned int xinit, unsigned int yinit)
{
  uint8_t x0 = xinit;
  uint8_t y0 =  yinit;   //

  //--------------------------------------------
  //display QRcode
  for (uint8_t y = 0; y < qrcode.size; y++) {
    for (uint8_t x = 0; x < qrcode.size; x++) {

      if (qrcode_getModule(&qrcode, x, y) == 0) {     //change to == 1 to make QR code with black background
        u8g2.setDrawColor(1);

        #ifdef  _QR_doubleSize
        //uncomment to double the QRcode. Comment to display normal code size
        u8g2.drawPixel(x0 + 2 * x,     y0 + 2 * y);
        u8g2.drawPixel(x0 + 2 * x + 1, y0 + 2 * y);
        u8g2.drawPixel(x0 + 2 * x,     y0 + 2 * y + 1);
        u8g2.drawPixel(x0 + 2 * x + 1, y0 + 2 * y + 1);
        #else
        //uncomment to display code in normal size.  Comment to double the QRcode
        u8g2.drawPixel(x0 + x, y0 + y);
        #endif

      } 
      else 
      {
        u8g2.setDrawColor(0);

        #ifdef  _QR_doubleSize
        //uncomment to double the QRcode. Comment to display normal code size
        u8g2.drawPixel(x0 + 2 * x,     y0 + 2 * y);
        u8g2.drawPixel(x0 + 2 * x + 1, y0 + 2 * y);
        u8g2.drawPixel(x0 + 2 * x,     y0 + 2 * y + 1);
        u8g2.drawPixel(x0 + 2 * x + 1, y0 + 2 * y + 1);
        #else
        //uncomment to display code in normal size.  Comment to double the QRcode
        u8g2.drawPixel(x0 + x, y0 + y);
        #endif
      }
    }
  }
  u8g2.sendBuffer();

}
void setup() {
  Serial.begin(115200);

  //--------------------------------------------
  //TEXT DISPLAY
  //--------------------------------------------
  // setup display
  u8x8.clearDisplay();
  u8x8.begin();
  u8x8.clearDisplay();
  //u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setFont(u8x8_font_torussansbold8_r);

  u8x8.drawString(0, 0, "VERBINDUNG      ");
  u8x8.drawString(0, 1, "FEHLGESCHLAGEN  ");
  u8x8.drawString(0, 2, "STARTE NEU...   ");
  u8x8.display();

  
  //--------------------------------------------
  //DELAY
  //--------------------------------------------
  delay(2000);

  //--------------------------------------------
  //TEXT DISPLAY
  //--------------------------------------------
  // setup display
  u8x8.clearDisplay();
  u8x8.drawString(0, 0, "VERBINDUNG1      ");
  u8x8.drawString(0, 1, "FEHLGESCHLAGEN2  ");
  u8x8.drawString(0, 2, "STARTE NEU3...   ");
  u8x8.display();
  //--------------------------------------------
  //DELAY
  //--------------------------------------------
  delay(2000);

  
  //--------------------------------------------
  //GRAPHICS DISPLAY TEXT
  //--------------------------------------------
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setContrast(220);                      // set OELD brightness(0->255)
  u8g2.setFont(u8g2_font_6x13_te );           // 9 pixel height
  //u8g2.setFont(u8x8_font_torussansbold8_r );
  u8g2.drawStr(0, 12, "Generating QR Code1");  // write something to the internal memory
  //u8g2.drawStr(0, 0, "Generating QR Code1");  // write something to the internal memory
  //u8g2.drawStr(0, 8, "Generating QR Code2");
  //u8g2.drawStr(0, 16, "Generating QR Code3");
  //u8g2.drawStr(0, 24, "Generating QR Code4");
  //u8g2.drawStr(0, 32, "Generating QR Code5");
  //u8g2.drawStr(0, 40, "Generating QR Code6");
  //u8g2.drawStr(0, 48, "Generating QR Code7");
  //u8g2.drawStr(0, 54, "Generating QR Code8");
  
  u8g2.sendBuffer();                          // transfer internal memory to the display

  //--------------------------------------------
  //DELAY
  //--------------------------------------------
  delay(2000);

  // Start time
  uint32_t dt = millis();

  //--------------------------------------------
  // Allocate memory to store the QR code.
  // memory size depends on version number
  uint8_t qrcodeData[qrcode_getBufferSize(QRcode_Version)];

  //--------------------------------------------
  //configure the text string to code
  //https://github.com/ArkEcosystem/AIPs/blob/master/AIPS/aip-13.md#simpler-syntax
  qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "Sajjad Hussain Bhatti");     //bitcoin address

  //--------------------------------------------
  // Print Code Generation Time
  dt = millis() - dt;
  Serial.print("QR Code Generation Time: ");
  Serial.print(dt);
  Serial.print("\n");

  //--------------------------------------------
  printQRCode();
  
  //--------------------------------------------
  //display generation time to OLED display
  u8g2.drawStr(0, 30, "Generation Time(ms)"); // write something to the internal memory
  u8g2.setCursor(0, 50);
  u8g2.print(dt);             // display time it took to generate code
  u8g2.sendBuffer();

  //--------------------------------------------
  //DELAY
  //--------------------------------------------
  delay(3000);




  //--------------------------------------------
  //GRAPHICS DISPLAY QR
  //--------------------------------------------
  //Turn on all pixels
  setPixelsHigh();

  displayQRCode(20, 2);

  //--------------------------------------------
  //DELAY
  //--------------------------------------------
  delay(2000);

  //--------------------------------------------
  //GRAPHICS DISPLAY TEXT
  //--------------------------------------------
  u8g2.clearDisplay();
  u8g2.clearBuffer();
  u8g2.drawStr(0, 20, "Sajjad Hussain Bhatti"); 

  //--------------------------------------------
  //DELAY
  //--------------------------------------------
  u8g2.sendBuffer();
  delay(2000);

  u8g2.clearDisplay();
  u8g2.clearBuffer();

  //--------------------------------------------
  //GRAPHICS DISPLAY QR
  //--------------------------------------------
  qrcode_initText(&qrcode, qrcodeData, QRcode_Version, QRcode_ECC, "Sajjad Hussain Tahiri");     //bitcoin address
  //Turn on all pixels
  setPixelsHigh();

  displayQRCode(20, 2);

  
  //--------------------------------------------
  //DELAY
  //--------------------------------------------
  delay(2000);
  
  //u8g2.end();
  // setup display
  
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
  
  u8.initDisplay();
  u8.clearDisplay();
  u8.begin();
  u8.initDisplay();
  u8.clear();
  //u8.clearBuffer();
  u8.clearDisplay();
//  u8.setFont(u8x8_font_chroma48medium8_r);
  u8.setFont(u8x8_font_torussansbold8_r);

    u8.drawString(0, 0, "VERBINDUNG      ");
    u8.drawString(0, 1, "FEHLGESCHLAGEN  ");
    u8.drawString(0, 2, "STARTE NEU...   ");
    u8.display();
  delay(2000);
  u8.clearDisplay();




}


void loop() {

  // delay(6000);
  //  u8g2.setContrast(1);    //dim display


}
