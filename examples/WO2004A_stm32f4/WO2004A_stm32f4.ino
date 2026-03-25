#include <Arduino.h>
#include <Wire.h>
#include <LCD_wo2004b.h>


#define SDA_pin PB9 // stm32f4
#define SCL_pin PB8 // stm32f4
#define RST_PIN PC3 // stm32f4

// #define SDA_pin 11 // esp32-S3 (IO11)
// #define SCL_pin 12 // esp32-S3 (IO12)
// #define RST_PIN 13 // esp32-S3 (IO13)

LCD_wo2004b lcd(RST_PIN); // I2C address: 0x78 << 1; Display size: 20x4

void setup()
{
  int8_t i = 32;

  Wire.setSDA(SDA_pin); // using pin STM32
  Wire.setSCL(SCL_pin); // using pin STM32
  Wire.begin();
  // Wire.begin(SDA_pin,SCL_pin); // using pin esp32
  // Wire.setClock (1000000);

  // TEST
  lcd.init();
  lcd.rotateOn();
  lcd.shiftStartLine(i);
  lcd.print("Hello");
  lcd.print("Display", 1, 6);
  lcd.print("WO2004b-TFH#", 2, 4);
  lcd.print("STARTING!", 3, 5);
  lcd.setCursor(3, 0);

  while (i > 0)
  {
    delay(130);
    i--;
    lcd.shiftStartLine(i);
  }
}

void loop() {}