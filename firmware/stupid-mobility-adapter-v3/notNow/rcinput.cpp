
#include <Arduino.h>  // So we can use serialEvent().
#include "config.h"
#include "safety.h"
#include <ServoInput.h>

// Steering Setup
const int SteeringSignalPin = SERVO_IN_STEER_PIN;  // MUST be interrupt-capable!
const int SteeringPulseMin = 1000;  // microseconds (us)
const int SteeringPulseMax = 2000;  // Ideal values for your servo can be found with the "Calibration" example

ServoInputPin<SteeringSignalPin> steering(SteeringPulseMin, SteeringPulseMax);

// Throttle Setup
const int ThrottleSignalPin = SERVO_IN_THROTTLE_PIN;  // MUST be interrupt-capable!
const int ThrottlePulseMin = 1000;  // microseconds (us)
const int ThrottlePulseMax = 2000;  // Ideal values for your servo can be found with the "Calibration" example

ServoInputPin<ThrottleSignalPin> throttle(ThrottlePulseMin, ThrottlePulseMax);

void startRCInput() {
  if (enableServoInput && !ServoInput.available()) {  // If we want to use servo input but it's not there, throw an error or wait for all signals to be ready
    error = true;
    errorMessage = "6 Servo Input Not Available";
  } else if (ServoInput.available()) {
    Serial.println("[INFO] Servo Input Waiting");
  }
}

void doRCInput() {
  actuatorSetpoint_request = steering.map(-128, 128);
  motorSpeed = throttle.map(-128, 128);
//  Serial.print("Steering: ");
//  Serial.print(actuatorSetpoint_request);
//  Serial.print("deg");
//  Serial.print(" | ");  // separator
//   Serial.print("Throttle: ");
//  Serial.print(motorSpeed);
//  Serial.print("% ");
//    if (motorSpeed >= 0) {
//    Serial.print("(Forward)");
//  }
//  else {
//    Serial.print("(Reverse)");
//  }
//
//  Serial.println();
  //  if (!ServoInput.available()) {
  //    error = true;
  //    errorMessage = "6 Servo Input Connection Lost";
  //  } else {
  //    int actuatorSetpoint_request = steering.map(-128, 128);
  //    int motorSpeed = throttle.map(-128, 128);
  //  }
}
