
#ifndef board_h
#define board_h

#include <Arduino.h>
#include <ODriveS1.h>
#include <LEDIndicator.h>

#define USB_BAUDRATE 115200  // Serial Baudrate for USB port
#define BAUDRATE 115200      // Must match what you configure on the ODrive (see docs for details)
#define MAX_ANGLE_X 45       // Maximum rotation angle for motor X
#define MAX_ANGLE_Y 45       // Maximum rotation angle for motor Y

#define HOMING_BUTTON 52     // Homing button pin
#define LED1_PIN 22          // LED indicator pin
#define LED1_COUNT 1        // Number of LEDs   

// Arduino Mega or Due - Serial1
// pin 19: RX - connect to ODrive TX
// pin 18: TX - connect to ODrive RX
HardwareSerial& odrive_serial_1 = Serial1;

// Arduino Mega or Due - Serial2
// pin 17: RX - connect to ODrive TX
// pin 16: TX - connect to ODrive RX
HardwareSerial& odrive_serial_2 = Serial2;

// ODriveS1 object for motor 1
ODriveS1 odrive1(odrive_serial_1, MAX_ANGLE_X, 1);

// ODriveS1 object for motor 2
ODriveS1 odrive2(odrive_serial_2, MAX_ANGLE_Y, 2);

// LEDIndicator object for indicator light
Adafruit_NeoPixel led1(LED1_COUNT, LED1_PIN, NEO_RGB + NEO_KHZ800);
LEDIndicator status_light(led1, LED1_COUNT);

#endif  // board_h