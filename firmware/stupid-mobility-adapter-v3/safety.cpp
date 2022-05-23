#include <Arduino.h>
#include "safety.h"
#include "config.h"

// we need to know if there is an error state from anywhere....
bool enable = false;
bool error = false;
bool enableServoInput = false;
String errorMessage = "";
//bool connectionEstablished = false;
int batteryVoltage;

void checkBattery(){
  batteryVoltage = map(analogRead(BATT_DIV_PIN), 0, 4095, 0, 30); // voltage divider 0-3v = 0-30v
}

void startSafety(){
  pinMode(errorPin, INPUT);
  pinMode(BATT_DIV_PIN, INPUT);
  checkBattery();
  checkEstop();
}

void doSafety(){
  checkEstop();
  checkBattery();
  if (error){
    motorSpeed = 0;
  }
}

// Function to check the error pin
// TODO: This might be better done with a pin interrupt
void checkEstop(){
  if (digitalRead(errorPin) == LOW){
    error = true;
    errorMessage = "1 Check ESTOP";
  }
  
  if (digitalRead(errorPin) == HIGH)
  {
    error = false;
    errorMessage = "";
  }
  
  if (error)
  {
    Serial.print("[ERROR] ");
    Serial.println(errorMessage);
    enable = false;
    enableServoInput = false;
  }
}
