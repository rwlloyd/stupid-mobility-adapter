#include <MCP4725.h>                  // https://github.com/RobTillaart/MCP4725
#include <Wire.h>                     // include the Wire Library - needed to communicate with the DAC
#include "config.h"
#include "safety.h"
#include "ps3.h"

// All speed and direction derived from the global motorSpeed variable from the PS3
uint32_t demand = 0; //Variable for the demanded speed (0-4095).
int motorSpeed;

MCP4725 MCP(MCP4725_I2C_ADDR);

void setupCurtis() 
{
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  Wire.begin();
  // ESP32
  MCP.begin(21, 22);
  Wire.setClock(3400000);
  //  AVR
  //  MCP.begin();
  //  Wire.setClock(800000);

  if (!MCP.isConnected())
  {
    error = true;
    errorMessage = "2 DAC Connection Error";
  } else
  {
    MCP.setValue(0);
  }
  digitalWrite(enablePin, enable);
}

void doCurtisControl() 
{
  if (!error)                                        // Using global error
  {
    // convert the signal from the controller to what the DAC expects
    demand = map(abs(motorSpeed), 0, 128, 0, 4095); 
  } else {
    demand = 0;         // if there's an error, lets make sure we stop  
  }
  // Set the enable Pin with the global enable
  digitalWrite(enablePin, enable);

  // Set the Direction
  if (motorSpeed >= 0) 
  {
    digitalWrite(dirPin, LOW);
  } else if (motorSpeed < 0) {
    digitalWrite(dirPin, HIGH);
  }
  // Set the motor voltage demand with the DAC. motorspeed comes from the controller
  MCP.setValue(demand);
}
