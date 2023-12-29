### ESP32 Pin Connection

#### TFT Display
VCC     ->  3V3
GND     ->  GND
CS      ->  GPIO21
RST     ->  GPIO4
DC      ->  GPIO15
SDA     ->  GPIO23
SCK     ->  GPIO18
LED     ->  3V3 (through 100R resistor)

#### RC522
3V3     ->  3V3
RST     ->  GPIO22
GND     ->  GND
IRQ     ->  (NC)
MISO    ->  GPIO19
MOSI    ->  GPIO23
SCK     ->  GPIO18
SDA     ->  GPIO5

#### Matrix Keypad
R1      ->  GPIO32
R2      ->  GPIO33
R3      ->  GPIO25
R4      ->  GPIO26
C1      ->  GPIO27
C2      ->  GPIO14
C3      ->  GPIO12
C4      ->  GPIO13