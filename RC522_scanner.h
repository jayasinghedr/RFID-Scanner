#ifndef RC522_SCANNER_H
#define RC522_SCANNER_H

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 22
#define SS_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void initialise_RC522 ()
{
//  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
//  Serial.println("Approximate your card to the reader...");
//  Serial.println();
}

#endif
