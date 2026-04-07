#include <Arduino.h>
// #include "Wire.h"
#include "LCD_wo2004b.h"

bool _init_lcd = false;
bool _rotate = false;

LCD_wo2004b::LCD_wo2004b(uint8_t rst_pin)
{
  _rst_pin = rst_pin;
}

void LCD_wo2004b::init()
{
  // Reset Display
  pinMode(_rst_pin, OUTPUT);
  digitalWrite(_rst_pin, LOW);
  delay(10);
  digitalWrite(_rst_pin, HIGH);
  delay(10);

  _init_lcd = true;

  // IS Instruction Table 0
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  WriteIns(0x20);               // Function Set /Select Table 0
  WriteIns(0x01);               // Clear display
  Wire.endTransmission();

  delay(20);

  Wire.beginTransmission(0x3C); // Write to adress 0x78
  WriteIns(0x90);               // Set DDRAM address
  WriteIns(0x00);               // Set DDRAM address
  WriteIns(0x06);               // Set Entry Mode
  // WriteIns (0x0C); //Display Control
  WriteIns(0x0F); // Display Control + blink + cursor

  // IS Instruction Table 1
  WriteIns(0x21); // Function Set : 0 0 1 0 0 0 IS2 IS1  /Select Table 1
  WriteIns(0x12); // Follows Control : 0 0 0 1 0 0 BS2 BS1    Bias select: 1/6
  // WriteIns (0x40); //Set ICON RAM Address
  WriteIns(0x30); // Power Control 1 : 0 0 1 1 0 0 0 SLEEP
  WriteIns(0x70); // Set booster ;V0 Control 2
  WriteIns(0x6F); // ICON/Power Control2
  Wire.endTransmission();

  delay(100);

  Wire.beginTransmission(0x3C); // Write to adress 0x78
  // IS Instruction Table 3
  WriteIns(0x23); // Function Set : 0 0 1 0 0 0 IS2 IS1  /Select Table 3
  WriteIns(0x81); // Contrast: VOP SET
  WriteIns(0x27); // Contrast: VOP SET
  WriteIns(0x82); // start line setting
  WriteIns(0x00); // start line setting  : 0 0 ST[5:0]
  WriteIns(0xA7); // Rgain set :1 0 1 0 RR[3:0]

  // IS Instruction Table 2
  WriteIns(0x22); // Function Set : 0 0 1 0 0 0 IS2 IS1  /Select Table 2
  WriteIns(0x60); // Set Display pattern : 0 1 1 0 0 0 INV AP
  WriteIns(0x13); // Set Display Mode :0 0 0 1 DH1 DH0 N2 N1  1/33D
  WriteIns(0x44); // Select CGRAM & COM/SEG direction

  // IS Instruction Table 3
  WriteIns(0x23); // Function Set : 0 0 1 0 0 0 IS2 IS1  /Select Table 3
  WriteIns(0x88); // Set 88H 4 times to entry IST test command mode
  WriteIns(0x88);
  WriteIns(0x88);
  WriteIns(0x88);
  Wire.endTransmission();

  Wire.beginTransmission(0x3C); // Write to adress 0x78
  WriteIns(0x28);               // Frame rate adjusting enable
  WriteIns(0xB2);               // 1st Frame rate control
  WriteIns(0xEF);               // 2nd LN[7:0]
  WriteIns(0x00);               // 3rd LN[15:8]  95Hz
  WriteIns(0x93);               // OSC Clock Select : Fosc/1
  WriteIns(0x99);               // OSC Divide Select : 750KHz
  WriteIns(0xE3);               // Exit IST test command

  // WriteIns (0x20); //Function Set /Select Table 0
  _init_lcd = false;
  Wire.endTransmission();
}

void LCD_wo2004b::clear()
{
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  Wire.write(0x80);             // Write instruction
  Wire.write(0x01);             // Instr - Clear
  Wire.endTransmission();
}

void LCD_wo2004b::setCursor(uint8_t _line, uint8_t _pos)
{
  uint8_t _adress = _line * 24 + _pos;
  if (_rotate)
    _adress = _adress + 4;
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  Wire.write(0x80);             // Write instruction
  Wire.write(0x90);             // Set DDRAM address
  Wire.write(0x80);             // Write instruction
  Wire.write(_adress);          // New Cursor adress
  Wire.endTransmission();
}

void LCD_wo2004b::home()
{
  if (_rotate)
    setCursor(0, 4);
  else
    setCursor(0, 0);
}

void LCD_wo2004b::print(String _text)
{
  uint8_t _len = _text.length();
  uint8_t _text_array[_len + 1];
  _text.toCharArray((char *)_text_array, _len + 1);
  WriteData(_text_array, _len);
}

void LCD_wo2004b::print(char _symbol)
{
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  Wire.write(0xC0); // Write Data to display
  Wire.write(_symbol);
  Wire.endTransmission();
}

void LCD_wo2004b::print(String _text, uint8_t _line, uint8_t _pos)
{
  setCursor(_line, _pos);
  uint8_t _len = _text.length();
  uint8_t _text_array[_len + 1];
  _text.toCharArray((char *)_text_array, _len + 1);
  WriteData(_text_array, _len);
}

void LCD_wo2004b::WriteIns(uint8_t Instr)
{
  if (_init_lcd == false)
    Wire.beginTransmission(0x3C); // Write to adress 0x78
  Wire.write(0x80);               // Write instruction
  Wire.write(Instr);              // Set New instruction
  if (_init_lcd == false)
    Wire.endTransmission();
}

void LCD_wo2004b::WriteData(const uint8_t *_data_array, uint8_t _len)
{
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  for (uint8_t i = 0; i < _len; i++)
  {
    Wire.write(0xC0); // Write Data to display
    Wire.write(_data_array[i]);
  }
  Wire.endTransmission();
}

void LCD_wo2004b::WriteData_char(const char *_data_array, size_t _len)
{
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  for (size_t i = 0; i < _len; i++)
  {
    Wire.write(0xC0); // Write Data to display
    Wire.write(_data_array[i]);
  }
  Wire.endTransmission();
}

void LCD_wo2004b::rotateOn()
{
  _rotate = true;
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  WriteIns(0x22);               // Select Table 2
  WriteIns(0x47);               // Select CGRAM & COM/SEG direction rotation
  Wire.endTransmission();
  setCursor(0, 0);
}

void LCD_wo2004b::shiftStartLine(uint8_t _shiftSL)
{
  Wire.beginTransmission(0x3C); // Write to adress 0x78
  WriteIns(0x23);               // Select Table 3
  WriteIns(0x82);               // start line setting
  WriteIns(_shiftSL);
  Wire.endTransmission();
}