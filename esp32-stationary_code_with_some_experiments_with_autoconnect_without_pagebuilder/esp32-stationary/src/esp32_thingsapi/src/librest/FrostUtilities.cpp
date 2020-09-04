#include "FrostUtilities.h"
#include <Arduino.h>
#include "mbedtls/md.h"

String toHEXSHA1(String serial) {
  int serialLength = serial.length();
  char* serialAsChar = new char[serialLength+1];
  serial.toCharArray(serialAsChar, serialLength+1);
  //Serial.print("String");
  //Serial.print(String(serialAsChar));
  

  byte shaResult[20]; //sha1 result has always 160bits
  
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA1;        
 
  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 0);
  mbedtls_md_starts(&ctx);
  mbedtls_md_update(&ctx, (const unsigned char *) serialAsChar, serialLength);
  mbedtls_md_finish(&ctx, shaResult);
  mbedtls_md_free(&ctx);
 
  //Serial.print("Hash from function");
  //print to HEX characters
  char sha1hex[2*sizeof(shaResult)+1];
  //Serial.print("sha1hexSize"+String(2*sizeof(shaResult)));
  for(int i= 0; i< sizeof(shaResult); i++){
      //char str[2];
 
      sprintf(&sha1hex[i*2], "%02x", (int)shaResult[i]);
      //sprintf(&sha1hex[38], "%02x", (int)shaResult[i]);
      //Serial.print(str);
  }
  //Serial.print(sha1hex);
  delete serialAsChar;
  return String(sha1hex);
}