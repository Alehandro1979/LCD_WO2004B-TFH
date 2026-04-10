#ifndef LCD_wo2004b_h
#define LCD_wo2004b_h

#ifdef __cplusplus
extern "C" {
#endif

#ifndef Arduino_h
#include <Arduino.h>
#endif // Arduino_h

#ifndef Wire_h
#include <Wire.h>
#endif // Wire_h




class LCD_wo2004b
{
public:
  LCD_wo2004b(uint8_t);
  void init();
  void clear();
  void print(String);
  void print(char);
  void print(String, uint8_t, uint8_t);
  void setCursor(size_t, size_t);
  void home();
  void rotateOn();
  void shiftStartLine(uint8_t);
  void WriteData_char(const char *, size_t);

private:
  uint8_t _rst_pin;
  bool _rotate;
  void WriteIns(uint8_t);
  void WriteData(const uint8_t *, uint8_t);
};

#ifdef __cplusplus
}
#endif

#endif