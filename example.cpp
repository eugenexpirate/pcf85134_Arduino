#include <Arduino.h>
#include <Wire.h>
#include "pcf85134.h"

// Description: Send one command to driver 
// Argument: Byte with command
// Return: none
void SendCommandToDriver(uint8_t commandByte) {
  Wire.beginTransmission(DRIVER_SLAVE_ADDRESS_DEFAULT);  
  // Next will say to driver that byte after this will be a command 
  Wire.write(DRIVER_CONTROL_BYTE_COMMAND);
  Wire.write(commandByte); 
  Wire.endTransmission();  
}

// Description: Send one command to driver 
// Argument: device address and address in driver segments memory
// Return: none
void SetDriverDataPointer(uint8_t deviceAddress, uint8_t addressPinterValue) {
  Wire.beginTransmission(DRIVER_SLAVE_ADDRESS_DEFAULT);  
  
  Wire.write(DRIVER_CONTROL_BYTE_COMMAND);
  Wire.write(deviceAddress); 

  Wire.write(DRIVER_CONTROL_BYTE_COMMAND);
  Wire.write(DRIVER_LOAD_DATA_POINTER(addressPinterValue));
  
  Wire.endTransmission(); 
}

// Description: Set all segment memory to 0 or 1 
// Argument: value to set 0 - set 0, other - to 1
// Return: none
void SetAllSegments(uint8_t value) {
  uint8_t byteValue = (value == 0) ? 0x00 : 0xFF;

  // DRIVER_DEVICE_SELECT_DEFAULT - A0, A1, A2 wired to VSS (GND)
  // Otherwise use DRIVER_DEVICE_SELECT macro
  SetDriverDataPointer(DRIVER_DEVICE_SELECT_DEFAULT, 0);
  Wire.beginTransmission(DRIVER_SLAVE_ADDRESS_DEFAULT);
  Wire.write(DRIVER_CONTROL_BYTE_DATA);

  // Next magic value 30 used for sending at last 240 bits
  // 30 * 8 bits = 240 bits 
  for(uint8_t i = 0; i < 30; ++i) {
    Wire.write(byteValue);
  }
  Wire.endTransmission();
}

// Description: Makes all segments on for a delay value and after turn off and wait the same delay 
// Argument: value of delay in ms
// Return: none
void CustomBlinkAllSegments(uint16_t delayValue_ms) {
  // makes all segments black
  SetAllSegments(1);
  delay(delayValue_ms);
  // makes all segments blank
  SetAllSegments(0);
  delay(delayValue_ms);    
  
}

void setup() {
  // PCF85134 driver starts in 1 ms
  delay(1);

  Wire.begin();
  SendCommandToDriver(DRIVER_MODE_SET(MODE_SET_ENABLE, MODE_SET_BIAS_DEFAULT, MODE_SET_MODE_1_4));
}

void loop() {
  CustomBlinkAllSegments(500);
}