#include <arduino.h>
#include "safety.h"
#include "config.h"

// we need to know if there is an error state from anywhere....
bool enable = false;
bool error = false;
String errorMessage = "";
//bool connectionEstablished = false;

void startSafety(){
  pinMode(errorPin, INPUT);
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
  }
}
