/*
   To Do

   Need a way to adjust the centre point of the steering. While we're at it, Min and Max too

*/

#include <Arduino.h>
#include "ps3.h"
#include <analogWrite.h>
#include <movingAvg.h>
#include <PID_v1.h>
//#include "comms.h"
#include "safety.h"
#include "config.h"


int actPosition;
int actuatorSetpoint_request = 0;
movingAvg actPositionAvg(4);                // define the moving average object

int Max = actCentre - (90 * 4);
int Min = actCentre + (90 * 4);
int vel = 200;

//Define Variables we'll be connecting to
//double actuatorSetpoint, actuatorInput, actuatorOutputLeft, actuatorOutputRight;
double actSetpoint, actInput, actOutput;

//Specify the links and initial tuning parameters
double Kp = ACT_P, Ki = ACT_I, Kd = ACT_D;
PID motorPID(&actInput, &actOutput, &actSetpoint, Kp, Ki, Kd, DIRECT);

void stopActuator() {
  analogWrite(ACT_R_PWM, 0);
  analogWrite(ACT_L_PWM, 0);
}

void setupActuator() {
  pinMode(ACT_FB, INPUT);
  pinMode(ACT_EN, OUTPUT);
  pinMode(ACT_R_PWM, OUTPUT);
  pinMode(ACT_L_PWM, OUTPUT);

  // Read the actuator position
  actInput = analogRead(ACT_FB);
  // begine the position averaging.
  actPositionAvg.begin();
  // set the inital setpoint to the centre position
  actSetpoint = actCentre;
  // Just to be on the safe side, make sur the actuator is disabled
  // stopActuator(); /// dont request a position then turn everything off....
  digitalWrite(ACT_EN, LOW);
  //turn the PID on
  motorPID.SetMode(AUTOMATIC);
  motorPID.SetOutputLimits(-255, 255); // default is 0-255
}

void doActuatorControl() {
  motorPID.Compute();
  if (!enable) {
    stopActuator();
    digitalWrite(ACT_EN, LOW);
  }
  else {
    digitalWrite(ACT_EN, HIGH);
    actSetpoint = map(actuatorSetpoint_request, -128, 128, Min, Max);
    //Serial.println(Setpoint);
    //Setpoint = actuatorSetpoint_request;
    actPosition = analogRead(ACT_FB);
    actInput = actPositionAvg.reading(actPosition);
    if (debugActuator) {
      Serial.print(actSetpoint);
      Serial.print(", ");
      Serial.print(actPosition);
      Serial.print(", ");
      Serial.print(actInput);
      Serial.print(", ");
      Serial.println(actOutput);
    }

    if (actInput <= actSetpoint + actDeadband && actInput >= actSetpoint - actDeadband) {
      stopActuator();
    }
    else if (actInput > actSetpoint + actDeadband) {
      digitalWrite(ACT_EN, HIGH);
      analogWrite(ACT_R_PWM, abs(actOutput));
      analogWrite(ACT_L_PWM, 0);
    }
    else if (actInput < actSetpoint - actDeadband) {
      // FOR this setup, left turn is extending the actuator to turn left
      digitalWrite(ACT_EN, HIGH);
      analogWrite(ACT_L_PWM, abs(actOutput));
      analogWrite(ACT_R_PWM, 0);
      //      // FOR this setup, left turn is extending the actuator to turn left
      //      Serial.println(Output);
      //      digitalWrite(ACT_EN, HIGH);
      //      analogWrite(ACT_L_PWM, constrain(abs(Output), 0, 255)); // this ones correct
      //      analogWrite(ACT_R_PWM, 0);
    }
  }
}
