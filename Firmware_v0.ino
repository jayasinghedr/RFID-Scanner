int curr_screen = 0;  //indicates the current screen

#include "TFT_display.h"
#include "RC522_scanner.h"
#include "matrix_keypad.h"
#include "WiFi_access.h"

// display screens
#define START           0
#define ACCESS_POINT    1
#define CONNECTING      2
#define READY           3
#define CLASS_ID        4
#define ID_SCAN         5
#define STUDENT_INFO    6
#define DEFAULT_SCREEN  7

#define CLASS_ID_LEN    3

int key_count = 0;
 
void setup() 
{
  Serial.begin(115200);
  
  //Startup the display
  initialise_display();

  initSPIFFS();
  
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);

  Serial.println(ssid);
  Serial.println(pass);

  if(initWiFi()) {
    Serial.println("\nConnected to the WiFi network");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    curr_screen = READY;  //device is ready
  }
  else {
    access_point();
    curr_screen = ACCESS_POINT;  //indicates the current screen

    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("ZeroCode-RFID", NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP); 

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", SPIFFS, "/");
    
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            writeFile(SPIFFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            // Write file to save value
            writeFile(SPIFFS, passPath, pass.c_str());
          }
        }
      }
      request->send(200, "text/plain", "Done. Device will restart");
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }
  //Connect to WiFI
  //initialise_WiFi();

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
    case START : {
      ;
    } break;
    case ACCESS_POINT: {
      ;
    } break;
    case READY: {
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

        // send class ID to the end point
        if ((WiFi.status() == WL_CONNECTED)) {
          if (send_class_id()) {   
            //class_id is correct -> go to ID scan screen
            id_scan_screen();
            curr_screen = ID_SCAN; 
          } else {
            //class_id is wrong -> display error and stay in same screen
            tft.fillScreen(ST7735_CYAN);
            tft.setFont(&FreeSansBold9pt7b);
            tft.setTextSize(0);  // set text size to 0 for custom font
            tft.setCursor(10, 60);  // Set position (x,-y)
            tft.setTextColor(ST7735_RED);  // Set color of text
            tft.println("Invalid Class ID");  // text
            tft.setFont();  // Reset to standard font

            delay(1500);
            class_id_screen();
          }
        }
        else {
          Serial.println("Connection lost");
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
