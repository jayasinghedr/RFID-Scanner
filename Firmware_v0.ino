#include "TFT_display.h"
#include "RC522_scanner.h"
#include "matrix_keypad.h"
#include "WiFi_access.h"

//indicates the current screen
int curr_screen = 0;  
 
void setup() 
{
  Serial.begin(115200);
  
  //Startup the display
  initialise_display();

  //Connect to WiFI
  initialise_WiFi();

  //Startup the RC522 RFID scanner
  initialise_RC522();

  //initialise the matrix keypad
  customKeypad.begin();

  tft.fillScreen(ST7735_WHITE);  // Fill screen

}

void loop() 
{
  customKeypad.tick(); 

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    if(e.bit.EVENT == KEY_JUST_RELEASED) {
      Serial.println((char)e.bit.KEY);
      if ((char)e.bit.KEY == '*') {       
        curr_screen = 0;
        tft.fillScreen(ST7735_WHITE);
      }
    }
  }
  delay(10);  //wait 10us

  //Screen and scanner state selection
  switch (curr_screen) {
    case 0: {
      start_screen();

      // Look for new cards
      if ( ! mfrc522.PICC_IsNewCardPresent()) 
      {
        return;
      }
      // Select one of the cards
      if ( ! mfrc522.PICC_ReadCardSerial()) 
      {
        return;
      }
      
      //Show UID on serial monitor
      Serial.print("UID tag :");
      String content= "";

      for (byte i = 0; i < mfrc522.uid.size; i++) 
      {
        //add a 0 for hex values less than 0x10 (16)
        if (mfrc522.uid.uidByte[i] < 0x10) {
          content.concat("0");
        }
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
      }
      
      content.toUpperCase();
      Serial.println(content.substring(0));
      
      Serial.print("Message : ");
      if (content == "402A871A") //change here the UID of the card/cards that you want to give access
      {
        Serial.println("Authorized access");
        Serial.println();

        curr_screen = 1;  //go to the next screen 
      }
    
      else   {
        Serial.println(" Access denied");
        Serial.println();
        delay(1000);
      }
    }  break;

    case 1: {
      tft.fillScreen(ST7735_CYAN);  // Fill screen
      tft.setFont(&FreeSansBold9pt7b);
      tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
      tft.setCursor(20, 20);  // Set position (x,-y)
      tft.setTextColor(ST7735_BLUE);
      tft.println("Student Info");

      tft.drawRect(10, 40, 140, 80, ST7735_RED);
      tft.setCursor(20, 55);
      tft.setTextColor(ST7735_BLACK);
      tft.println("Name: ");
      tft.println("ID: ");
      tft.setFont();

       curr_screen = 2;  //go to the next state (otherwise screen will flicker
    }  break;
    case 2: {
      ;   
    }
  }

} 
