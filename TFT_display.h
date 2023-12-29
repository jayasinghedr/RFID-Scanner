#ifndef TFT_DISPLAY
#define TFT_DISPLAY

#include <Adafruit_GFX.h>     // Include core graphics library
#include <Adafruit_ST7735.h>  // Include Adafruit_ST7735 library to drive the display


// set up pins we are going to use to talk to the screen
#define TFT_DC      15        // register select (stands for Data Control perhaps!)
#define TFT_RST     4         // Display reset pin, you can also connect this to the ESP32 reset
                              // in which case, set this #define pin to -1!
#define TFT_CS      21        // Display enable (Chip select), if not enabled will not talk on SPI bus (GPIO 5)

// initialise the routine to talk to this display with these pin connections (as we've missed off
// TFT_SCLK and TFT_MOSI the routine presumes we are using hardware SPI and internally uses MOSI - 23 and SCK - 18
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// Add a custom font
#include <Fonts/FreeSerif18pt7b.h>  
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

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

void start_screen()
{
  //Draw an arrow
  tft.fillRect(76, 32, 8, 16, ST7735_RED);
  tft.fillTriangle(80, 16, 88, 32, 72, 32, ST7735_RED);

  //print scan message
  tft.setFont(&FreeSansBold9pt7b);
  tft.setTextSize(0);  // Set text size. We are using custom font so you should always set text size as 0
  tft.setCursor(20, 80);  // Set position (x,-y)
  tft.setTextColor(ST7735_WHITE);  // Set color of text. First is the color of text and after is color of background
  tft.println("Scan ID Here");  // Print a text or value
  tft.setFont();  // Reset to standard font, to stop using any custom font previously set
}

#endif
