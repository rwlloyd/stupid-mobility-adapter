
#include "comms.h"
#include "network.h"
#include "ps3.h"
#include "rcinput.h"
#include "config.h"
//#include "dc_motor.h"
#include "curtis.h"
#include "actuator.h"
#include "safety.h"
#include <Arduino.h> // Necessary to make vscode problem free. vscode 1.50.1 bug workaround.

// bastard thing wont fit onto a stock esp32 uses 1459006 bytes (111%) Max size is 1310720
// You have to use a non-standard partitioning scheme for the esp32. Doubtful that the new version will fit on a nano either.

void setup() {
  startSafety();
  startComms();
  startNetwork();
  setupPs3();
  setupActuator();
  //setupMobility();
  setupCurtis(); 
  //startRCInput();
  Serial.println("[INFO] Setup Completed");
}

void loop() {
  doSafety();
  doNetworking(); 
  checkPs3();
  //checkConnection();      // comms.cpp - Only needed when relying on the serial connection for safety critical control
  checkSerialComs();      // comms.cpp - Needed if recieving serial data for control. So, not yet.
  if (error) {
    stopActuators();
    doCurtisControl();
    //stopMotor();          // for use with pwm motor driver.
  }
  else {
    doActuatorControl();
    doCurtisControl();
    //doMobilityControl();  // for use with a pwm motor driver
  }
}
