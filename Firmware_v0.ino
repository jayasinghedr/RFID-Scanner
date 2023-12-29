#include "TFT_display.h"
#include "RC522_scanner.h"
#include "matrix_keypad.h"
 
void setup() 
{
  Serial.begin(9600);
  
  //Startup the display
  initialise_display();

  //Startup the RC522 RFID scanner
  initialise_RC522();

  //initialise the matrix keypad
  customKeypad.begin();

  tft.fillScreen(ST7735_BLACK);  // Fill screen

}

void loop() 
{
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
  //byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "40 2A 87 1A") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();

    tft.fillScreen(ST7735_CYAN);  // Fill screen
    tft.setFont(&FreeSansBold9pt7b);
    tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
    tft.setCursor(20, 20);  // Set position (x,-y)
    tft.setTextColor(ST7735_BLUE);
    tft.println("Student Info");

    tft.drawRect(10, 40, 140, 80, ST7735_RED);
    tft.setCursor(12, 55);
    tft.setTextColor(ST7735_BLACK);
    tft.println("Name: ");
    tft.println("ID: ");
    tft.setFont();
    
    delay(3000);
    tft.fillScreen(ST7735_BLACK); 
  }
 
 else   {
    Serial.println(" Access denied");
    Serial.println();
    delay(1000);
  }
} 
