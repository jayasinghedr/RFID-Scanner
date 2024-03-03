#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <Adafruit_GFX.h>     // Include core graphics library
#include <Adafruit_ST7735.h>  // Include Adafruit_ST7735 library to drive the display


// set up pins we are going to use to talk to the screen
#define TFT_DC      15        // register select (stands for Data Control perhaps!)
#define TFT_RST     4         // Display reset pin, you can also connect this to the ESP32 reset
                              // in which case, set this #define pin to -1!
#define TFT_CS      21        // Display enable (Chip select), if not enabled will not talk on SPI bus (GPIO 5)

#define BAT_READ    34        // Analogue battery read

// initialise the routine to talk to this display with these pin connections (as we've missed off
// TFT_SCLK and TFT_MOSI the routine presumes we are using hardware SPI and internally uses MOSI - 23 and SCK - 18
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Add a custom font
#include <Fonts/FreeSerif18pt7b.h>  
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

int battery_val = 0;

void initialise_display()
{
  // Display setup:

  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);  // Initialize a ST7735S chip, black tab

  tft.fillScreen(ST7735_YELLOW);  // Fill screen with black

  tft.setRotation(1);  // Set orientation of the display. Values are from 0 to 3. If not declared, orientation would be 0,
                         // which is portrait mode.

  tft.setTextWrap(false);  // By default, long lines of text are set to automatically “wrap” back to the leftmost column.
                           // To override this behavior (so text will run off the right side of the display - useful for
                           // scrolling marquee effects), use setTextWrap(false). The normal wrapping behavior is restored
                           // with setTextWrap(true).


  tft.setFont(&FreeSansBold12pt7b);
  tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(20, 70);  // Set position (x,-y)
  tft.setTextColor(ST7735_BLUE);  // Set color of text. First is the color of text and after is color of background
  tft.println("ZeroCode");  // Print a text or value

  // Stop using a custom font:
  tft.setFont();  // Reset to standard font, to stop using any custom font previously set

  delay(3000);
}

void access_point () {
  //display this screen when the device works as an access point
  tft.fillScreen(ST7735_GREEN);  // Fill screen

  tft.setFont(&FreeMonoBold9pt7b);
  tft.setCursor(20, 30);
  tft.setTextColor(ST7735_RED);
  tft.println("No WiFi!");
  tft.setFont();

  tft.setFont(&FreeMono9pt7b);
  tft.setCursor(20, 50);
  tft.setTextColor(ST7735_BLACK);
  tft.println("HotSpot Mode ON");
  tft.setFont();

}

void battery_indicator ()
{
  // place battery charge level indicator at the top right corner
  tft.fillRect(134, 5, 21, 12, ST7735_BLUE);
  tft.fillRect(155, 9, 3, 4, ST7735_BLUE);

  // battery percentage
  tft.setCursor(136, 7);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.println("85%");

  //Serial.println(battery_val);
  //delay(1000);
}

void class_id_screen() {
  tft.fillScreen(ST7735_CYAN);  // Fill screen
  
  //Display text
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);  // set text size to 0 for custom font
  tft.setCursor(40, 30);  // Set position (x,-y)
  tft.setTextColor(ST7735_BLACK);  // Set color of text
  tft.println("Class ID");  // text
  tft.setFont();  // Reset to standard font

  tft.drawRect(40, 80, 80, 30, ST7735_MAGENTA);
}

void id_scan_screen()
{
  tft.fillScreen(ST7735_WHITE);  // Fill screen
  
  //Draw an arrow
  tft.fillRect(76, 32, 8, 16, ST7735_RED);
  tft.fillTriangle(80, 16, 88, 32, 72, 32, ST7735_RED);

  // put battery indicator
  // battery_indicator();

  //print scan message
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(20, 80);  // Set position (x,-y)
  tft.setTextColor(ST7735_BLACK);  // Set color of text. First is the color of text and after is color of background
  tft.println("Scan ID Here");  // Print a text or value
  tft.setFont();  // Reset to standard font, to stop using any custom font previously set
}

void student_info_screen()
{
  tft.fillScreen(ST7735_CYAN);  // Fill screen
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(20, 20);  // Set position (x,-y)
  tft.setTextColor(ST7735_BLUE);
  tft.println("Student Info");
  tft.setFont();

  tft.drawRect(10, 40, 140, 80, ST7735_RED);
  tft.setFont(&FreeMono9pt7b);
  tft.setCursor(20, 55);
  tft.setTextColor(ST7735_BLACK);
  tft.println("Name: ");
  tft.setCursor(20, 80);
  tft.println("ID: ");
  tft.setFont();
}

#endif
