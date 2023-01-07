#include <Arduino.h>
#include <BTS7960.h>
//#include <analogWrite.h>
#include <movingAvg.h>                  // https://github.com/JChristensen/movingAvg
#include "safety.h"
#include "config.h"
#include "comms.h"
#include "ps3.h"

#if (NANO || ALL_BTS7960)
BTS7960 motorController(MOT_EN, MOT_EN, MOT_L_PWM, MOT_R_PWM);

int lastMotorSpeed;
int motorDesired;

movingAvg motorDesiredAvg(10);                // define the moving average object

void setupMobility(){
  //Brake Pin
  pinMode(BRAKE_PIN, OUTPUT);
  pinMode(MOT_EN, OUTPUT);
  pinMode(MOT_L_PWM, OUTPUT);
  pinMode(MOT_R_PWM, OUTPUT);

  motorDesiredAvg.begin();
  motorController.Stop();
  motorController.Disable();
}

void stopMotor(){
    digitalWrite(BRAKE_PIN, enable);
    motorController.Stop();
    motorController.Disable();
}

void doMobilityControl(){
    if (!enable) {
        stopMotor();
    }
    else {
        digitalWrite(BRAKE_PIN, enable);
        motorController.Enable();
        
        motorDesired = motorDesiredAvg.reading(motorSpeed);
        if (motorDesired == 0) {
            motorController.Stop();
        }
        if (motorDesired < 0) {
            motorController.Enable();
            motorController.TurnRight(map(motorSpeed, 128, 0, motorMin, speedLimit));
        }
        if (motorDesired > 0) {
            motorController.Enable();
            motorController.TurnLeft(map(motorSpeed, -128, 0, motorMin, speedLimit));
        }
    }
}

#endif
