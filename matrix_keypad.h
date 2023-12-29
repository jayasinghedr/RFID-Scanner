#ifndef MATRIX_KEYPAD
#define MATRIX_KEYPAD

#include "Adafruit_Keypad.h"

const byte ROWS = 4; // rows
const byte COLS = 4; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

//byte rowPins[ROWS] = {19, 18, 32, 33}; //connect to the row pinouts of the keypad
//byte colPins[COLS] = {25, 23, 22, 21}; //connect to the column pinouts of the keypad

byte rowPins[ROWS] = {32, 33, 25, 26}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {27, 14, 12, 13}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

#endif
