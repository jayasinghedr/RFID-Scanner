#include "TFT_display.h"
#include "RC522_scanner.h"
#include "matrix_keypad.h"
//#include "WiFi_access.h"

// display screens
#define START           0
#define CLASS_ID        1
#define ID_SCAN         2
#define STUDENT_INFO    3
#define DEFAULT_SCREEN  4

#define CLASS_ID_LEN    4

int curr_screen = 0;  //indicates the current screen
String class_id = ""; //store class ID from keypad
int key_count = 0;

uint32_t chip_id = 0;
 
void setup() 
{
  Serial.begin(115200);
  
  //Startup the display
  initialise_display();

  //Connect to WiFI
  // initialise_WiFi();

  //Startup the RC522 RFID scanner
  initialise_RC522();

  //initialise the matrix keypad
  customKeypad.begin();

}

void loop() 
{
  customKeypad.tick(); 

  battery_val = analogRead(BAT_READ);

  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    if(e.bit.EVENT == KEY_JUST_RELEASED) {
      Serial.println((char)e.bit.KEY);
      //go be to scan screen
      if ((char)e.bit.KEY == '*') {  
        id_scan_screen();     
        curr_screen = ID_SCAN;
      }

      if (curr_screen == CLASS_ID) {
        class_id.concat(String((char)e.bit.KEY));

        tft.setCursor(45, 82);
        tft.setTextColor(ST7735_BLACK);
        tft.setTextSize(3);
        tft.println(class_id);

        key_count++;       
      }
    }
  }
  delay(10);  //wait 10us

  //Screen and scanner state selection
  switch (curr_screen) {
    case START: {
      class_id_screen();

      //get chip ID
      for(int i=0; i<17; i=i+8) {
	      chip_id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
	    }
      Serial.print("Chip ID: "); Serial.println(chip_id);

      //go to CLASS_ID screen
      curr_screen = CLASS_ID;

    } break;

    case CLASS_ID: {
      //check if the entered class ID is correct
      //correct -> go to SCAN_ID screen
      //incorrect -> display invalid ID and go back to CLASS_ID screen
      if (key_count >= CLASS_ID_LEN) {
        delay(500);
        if (class_id == "1234") {         
          id_scan_screen();
          curr_screen = ID_SCAN;  

        } else {
          tft.fillScreen(ST7735_CYAN);

          //display invalid class ID
          tft.setFont(&FreeSansBold9pt7b);
          tft.setTextSize(0);  // set text size to 0 for custom font
          tft.setCursor(10, 60);  // Set position (x,-y)
          tft.setTextColor(ST7735_RED);  // Set color of text
          tft.println("Invalid Class ID");  // text
          tft.setFont();  // Reset to standard font

          delay(1500);

          //go back to previous screen
          class_id_screen();
        }
        key_count = 0;
        class_id = "";
      }
    } break;

    case ID_SCAN: {

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

        curr_screen = STUDENT_INFO;  //go to the next screen 
      }
    
      else   {
        Serial.println(" Access denied");
        Serial.println();
        delay(1000);
      }
    }  break;

    case STUDENT_INFO: {

      student_info_screen();
      curr_screen = DEFAULT_SCREEN;  //go to the next state (otherwise screen will flicker)

    }  break;

    case DEFAULT_SCREEN: {
      ;   
    }
  }

} 
