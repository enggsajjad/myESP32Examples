#include <Wire.h>
#include "SSD1306.h"
#include <qrcode.h>
#include "myheader.h"
#include <U8x8lib.h>
#define RST 16
//SSD1306 display(0x3c, 21, 22);
SSD1306 disp(0x3c, 4, 15);//78
//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
// initialise the OLED which is used


QRcode qrcode (&disp);

void setup() {
   delay(1000);

  pinMode(RST, OUTPUT);
  digitalWrite(RST,HIGH);

  Serial.begin(115200);
  // setup display
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
  u8x8.begin();
  u8x8.setFont(u8x8_font_torussansbold8_r);
  u8x8.clearDisplay();
  u8x8.clear();
  u8x8.drawString(0, 0, "123456789012345678");
  u8x8.drawString(0, 1, "ABCDEFGHU");
  u8x8.drawString(0, 2, "ABCDEFGHU");
  u8x8.drawString(0, 3, "ABCDFE SAJJAD");
  u8x8.drawString(0, 4, "1234567890"); 
  u8x8.drawString(0, 5, "1234567890");
  u8x8.drawString(0, 6, "1234567890");
  u8x8.drawString(0, 7, "Text1");
  
  delay(2000);

  digitalWrite(RST,LOW);
  delay(100);
  digitalWrite(RST,HIGH);
  delay(100);

    
  Serial.println("Hello IntEnseChol!");
  disp.init();

  
  qrcode.init();
  qrcode.create("Hello from Sajjad");
  
  delay(3000);
  disp.clear();


  
  //disp.setFont(ArialMT_Plain_16);//SansSerif_bold_8
  //disp.setFont(Crushed_Regular_13);//21 Characters x 6 Lines perfect
  //disp.drawString(0, 0, "Crushed_Regular_13");
  
  //disp.setFont(Syncopate_Bold_14);//11 Characters x 6 Lines
  //disp.setFont(Syncopate_Bold_12);//15 Characters x 6 Lines
  //disp.setFont(URW_Bookman_L_Light_14);//14 Characters x 6 Lines
  //disp.setFont(URW_Bookman_L_Light_13);//16 Characters x 6 Lines perfect
  //disp.drawString(0, 0, "URW_Bookman_L_Light_13");
  /*
  disp.drawString(0, 10, "123456789012345678901");
  disp.drawString(0, 20, "ABCDEFGHU");
  disp.drawString(0, 30, "ABCDEFGHU");
  disp.drawString(0, 40, "ABCDFE SAJJAD");
  disp.drawString(0, 50, "1234567890");
  */

  /*disp.setFont(URW_Bookman_L_Light_10);//16 Characters x 7 Lines perfect
  disp.drawString(0, 0, "URW_Bookman_L_Light_10");
  disp.drawString(0, 9, "123456789012345678901");
  disp.drawString(0, 18, "ABCDEFGHU");
  disp.drawString(0, 27, "ABCDEFGHU");
  disp.drawString(0, 36, "ABCDFE SAJJAD");
  disp.drawString(0, 45, "1234567890"); 
  disp.drawString(0, 54, "1234567890");  
  */
  /*
  disp.drawString(0, 0, "ABCDFE SAJJAD");
  disp.drawString(0, 8, "123456789012345678901");
  disp.drawString(0, 16, "ABCDEFGHU");
  disp.drawString(0, 24, "ABCDEFGHU");
  disp.drawString(0, 32, "ABCDFE SAJJAD");
  disp.drawString(0, 40, "1234567890");
  disp.drawString(0, 48, "ABCDFE SAJJAD");
  disp.drawString(0, 56, "ABCDFE SAJJAD");
  */


  //disp.setFont(URW_Bookman_L_Light_10);//16 Characters x 7 Lines perfect
  //diy.drawString(0, 0, "URW_Bookman_L_Light_10");splaURW_Bookman_L_Demi_Bold_10
  //disp.setFont(URW_Bookman_L_Demi_Bold_10);//16 Characters x 8 Lines perfect
  //disp.drawString(0, 0, "URW_Bookman_L_Demi_Bold_10");
  
  //disp.setFont(URW_Bookman_L_Demi_Bold_9);//16 Characters x 8 Lines perfect
  //disp.drawString(0, 0, "URW_Bookman_L_Demi_Bold_9");
  
  //disp.setFont(URW_Bookman_L_Light_9);//16 Characters x 8 Lines perfect
  //disp.drawString(0, 0, "URW_Bookman_L_Light_9");
  
  
  //disp.setFont(ArialMT_Plain_10);//16 Characters x 8 Lines perfect
  //disp.drawString(0, 0, "ArialMT_Plain_10");

  disp.setFont(Syncopate_Bold_9);//16 Characters x 8 Lines perfect
  disp.drawString(0, 0, "Syncopate_Bold_9");
  
  disp.drawString(0, 8, "123456789012345678");
  disp.drawString(0, 16, "ABCDEFGHU");
  disp.drawString(0, 24, "ABCDEFGHU");
  disp.drawString(0, 32, "ABCDFE SAJJAD");
  disp.drawString(0, 40, "1234567890"); 
  disp.drawString(0, 47, "1234567890");
  disp.drawString(0, 56, "Graphics1");
  
  disp.display(); 

  delay(3000);
  //clear graphics oled
  disp.clear();
  /////////////////////////////////////////////////////////////////////////
  digitalWrite(RST,LOW);
  delay(100);
  digitalWrite(RST,HIGH);
  delay(100);
  
  //display using Textbased oled
  // print FW version on display for 2 seconds
  //initialize another oled
  // setup display
  U8X8_SSD1306_128X64_NONAME_SW_I2C u8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);
  u8.begin();
  u8.setFont(u8x8_font_torussansbold8_r);
  u8.clearDisplay();
  u8.clear();
  u8.drawString(0, 0, "123456789012345678");
  u8.drawString(0, 1, "ABCDEFGHU");
  u8.drawString(0, 2, "ABCDEFGHU");
  u8.drawString(0, 3, "ABCDFE SAJJAD");
  u8.drawString(0, 4, "1234567890"); 
  u8.drawString(0, 5, "1234567890");
  u8.drawString(0, 6, "1234567890");
  u8.drawString(0, 7, "Text1");
  
  delay(2000);
  u8.clearDisplay();

  //  messages on display
  u8x8.drawString(0, 0, "123456789012345678");
  u8x8.drawString(0, 1, "2213ABCDEFGHU");
  u8x8.drawString(0, 2, "3232ABCDEFGHU");
  u8x8.drawString(0, 3, "232ABCDFE SAJJAD");
  u8x8.drawString(0, 4, "as1234567890"); 
  u8x8.drawString(0, 5, "afa1234567890");
  u8x8.drawString(0, 6, "afa1234567890");
  u8x8.drawString(0, 7, "Text2");
  
  delay(2000);

  //clear TextBased oled and try graphics again
  u8x8.clearDisplay();
  /////////////////////////////////////////////////////////////////////////
  disp.init();
  qrcode.init();
  qrcode.create("Hello from Sajjad");
  
  delay(3000);
  disp.clear();
  disp.setFont(Syncopate_Bold_9);//16 Characters x 8 Lines perfect
  disp.drawString(0, 0, "Syncopate_Bold_9");
  
  disp.drawString(0, 8, "123456789012345678");
  disp.drawString(0, 16, "ABCDEFGHU");
  disp.drawString(0, 24, "ABCDEFGHU");
  disp.drawString(0, 32, "ABCDFE SAJJAD");
  disp.drawString(0, 40, "1234567890"); 
  disp.drawString(0, 47, "1234567890");
  disp.drawString(0, 56, "Graphics2");
  
  disp.display(); 
  
}
 
void loop() {}
