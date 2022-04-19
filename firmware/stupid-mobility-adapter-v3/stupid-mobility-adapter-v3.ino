
#include "comms.h"
#include "network.h"
#include "ps3.h"
#include "config.h"
//#include "dc_motor.h"
#include "curtis.h"
#include "actuator.h"
#include "safety.h"
#include <Arduino.h> // Necessary to make vscode problem free. vscode 1.50.1 bug workaround.

// bastard thing wont fit onto a stock esp32 uses 1459006 bytes (111%) Max size is 1310720

void setup() {
  startSafety();
  startComms();
  startNetwork();
  setupPs3();
  setupActuator();
  //setupMobility();
  setupCurtis(); 
  Serial.println("[INFO] Setup Completed");
}

void loop() {
  checkEstop();
  //checkConnection(); // comms.cpp - Only needed when relying on the serial connection for safety critical control
  //checkSerialComs(); // comms.cpp - Needed if recieving serial data for control. So, not yet.
  checkPs3();
  //doNetworking();  //not necessary if we cant have ota.... yet?
  if (error) {
    //stopMotor();
    stopActuator();
    doCurtisControl();
  }
  else {
    doActuatorControl();
    //doMobilityControl();
    doCurtisControl();
  }
}
