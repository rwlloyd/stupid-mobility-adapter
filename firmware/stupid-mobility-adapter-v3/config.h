/*
 * NOTES
 * 
 * When flashing the microcontroller, make sure that the main battery power is disconnected and the ESTOP is pushed

*/

#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

// variables that lots of things want to access anyway
extern int motorSpeed;
extern int actuatorSetpoint_request;


#define errorPin 36
#define BATT_DIV_PIN 35 // 15 can't be used while BT and Wifi are enabled... https://docs.espressif.com/projects/esp-idf/en/release-v4.0/api-reference/peripherals/adc.html

// Select configuration below
// #define NANO               // AVR microcontroller connecter to two BTS7960 motor drivers. Probably wont work right now. see https://github.com/rwlloyd/Scooterbot for more. 
// #define ALL_BTS7960        // ESP32 microcontroller dual bts7960. 
#define CURTIS                // esp32 microcontroller curtis unit bts7960 actuator

#ifdef NANO
/*
 * Pinout configuration for Arduino nano controlling two IBT_2 Motor drivers for actuator 
 * and speed control, relay board for. probably might work.
*/
  // for Brake
  #define BRAKE_PIN 11
  #define errorPin 2
  // for motor, left is backwards
  #define MOT_R_EN 3
  #define MOT_L_EN 4
  #define MOT_L_PWM 6
  #define MOT_R_PWM 5
  #define MOT_L_IS A1
  #define MOT_R_IS A2
  #define motorCentre 128
  #define motorMin 50
  #define motorMax 255
  #define speedLimit 128
  // For actuator, extend is....
  #define ACT_R_EN 7
  #define ACT_L_EN 8
  #define ACT_R_PWM 9
  #define ACT_L_PWM 10
  #define ACT_FB A0
#endif

#ifdef ALL_BTS7960
/*
 * For ESP32 pinout with dual IBT_2 on v3 board 
 * BTS0 is the motor connection BTS1 is the Actuator. 
 * ACT1_FB is used for actuator controld to make adding a curtis unit to the mix simpler.
*/
// for Brake
#define BRAKE_PIN 13
// for motor, left is backwards
#define MOT_EN 32 // pins are now tied together
#define MOT_L_PWM 25
#define MOT_R_PWM 33
//#define MOT_IS 35
#define motorMin 50
#define motorMax 255
#define speedLimit 255
#endif

#ifdef CURTIS
/*
 * For ESP32 with curtis unit and IBT_2 actuator FB as ALL_BTS7960
*/
#define dirPin 19
#define enablePin 18
#define MCP4725_I2C_ADDR 0x60  // or 0x62 or 0x63
#define CURTIS_DEADBAND 5
#endif


// For the moment the actuator is always the same
// For actuator, extend is....
#define ACT_EN 27
#define ACT_R_PWM 14
#define ACT_L_PWM 12
#define ACT_FB 34 // ACT1_FB

#define ACT_P 2
#define ACT_I 0.1
#define ACT_D 0

#define actCentre 889              // 263 on 0-1024 scale
#define actDeadband 5              //Stops the actuator hunting quite so much

// Communication 
#define MESSAGELENGTH 4
#define BAUDRATE 115200
#define COMCHECKTIME 250

#define SERVO_IN_THROTTLE_PIN 16
#define SERVO_IN_STEER_PIN 17

#endif
