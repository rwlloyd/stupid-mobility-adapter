/*
   To Do

   Need a way to adjust the centre point of the steering. While we're at it, Min and Max too

*/

#include <Arduino.h>
//#include <analogWrite.h>
#include <movingAvg.h>
#include <PID_v1.h>
#include "comms.h"
#include "safety.h"
#include "config.h"
#include "ps3.h"

// Steering Actuator Variables
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

// Tool Actuator Variables
int toolPosition;
int toolSetpoint_request = 0;
movingAvg toolPositionAvg(4);                // define the moving average object

// int toolMax = toolCentre - (90 * 4);
// int toolMin = toolCentre + (90 * 4);
int toolvel = 200;

//Define Variables we'll be connecting to
//double actuatorSetpoint, actuatorInput, actuatorOutputLeft, actuatorOutputRight;
double toolSetpoint, toolInput, toolOutput;

//Specify the links and initial tuning parameters
double Kpt = ACT_P, Kit = ACT_I, Kdt = ACT_D;
PID toolPID(&toolInput, &toolOutput, &toolSetpoint, Kpt, Kit, Kdt, DIRECT);

void stopActuators() {
  // Steering Actuator 
  analogWrite(ACT_R_PWM, 0);
  analogWrite(ACT_L_PWM, 0);
  // Tool Actuator
  analogWrite(TOOL_R_PWM, 0);
  analogWrite(TOOL_L_PWM, 0);
}

void setupActuator() {
  pinMode(ACT_FB, INPUT);
  pinMode(ACT_EN, OUTPUT);
  pinMode(ACT_R_PWM, OUTPUT);
  pinMode(ACT_L_PWM, OUTPUT);

  pinMode(TOOL_FB, INPUT);
  pinMode(TOOL_EN, OUTPUT);
  pinMode(TOOL_R_PWM, OUTPUT);
  pinMode(TOOL_L_PWM, OUTPUT);

  // Read the actuator position
  actInput = analogRead(ACT_FB);
  toolInput = analogRead(TOOL_FB);
  // begine the position averaging.
  actPositionAvg.begin();
  toolPositionAvg.begin();
  // set the inital setpoint to the centre position
  actSetpoint = actCentre;
  toolSetpoint_request = 1750; // toolMax; bad idea to make the tool move by default at startup
  // Just to be on the safe side, make sur the actuator is disabled
  // stopActuator(); /// dont request a position then turn everything off....
  digitalWrite(ACT_EN, LOW);
  digitalWrite(TOOL_EN, LOW);
  //turn the PID on
  motorPID.SetMode(AUTOMATIC);
  motorPID.SetOutputLimits(-255, 255); // default is 0-255

  toolPID.SetMode(AUTOMATIC);
  toolPID.SetOutputLimits(-255, 255); // default is 0-255
}

void doActuatorControl() {
  motorPID.Compute();
  toolPID.Compute();
  if (!enable) {
    stopActuators();
    digitalWrite(ACT_EN, LOW);
    digitalWrite(TOOL_EN, LOW);
  }
  else {
    digitalWrite(ACT_EN, HIGH);
    digitalWrite(TOOL_EN, HIGH);
    actSetpoint = map(actuatorSetpoint_request, -128, 128, actMin, actMax);
    toolSetpoint = constrain(toolSetpoint_request, toolMin, toolMax); //// Not really happy with this. needs better control
    // toolSetpoint = map(toolSetpoint_request, -128, 128, toolMin, toolMax);
    //Serial.println(Setpoint);
    actPosition = analogRead(ACT_FB);
    toolPosition = analogRead(TOOL_FB);
    actInput = actPositionAvg.reading(actPosition);
    toolInput = toolPositionAvg.reading(toolPosition);
    if (debugActuator) {
      Serial.print(actSetpoint);
      Serial.print(", ");
      Serial.print(actPosition);
      Serial.print(", ");
      Serial.print(actInput);
      Serial.print(", ");
      Serial.print(actOutput);
      Serial.print(", ");
      Serial.print(toolSetpoint);
      Serial.print(", ");
      Serial.print(toolPosition);
      Serial.print(", ");
      Serial.print(toolInput);
      Serial.print(", ");
      Serial.println(toolOutput);
    }

    // Set PWM for Steering
    if (actInput <= actSetpoint + actDeadband && actInput >= actSetpoint - actDeadband) {
      analogWrite(ACT_R_PWM, 0);
      analogWrite(ACT_L_PWM, 0);
    }
    else if (actInput > actSetpoint + actDeadband) {
      digitalWrite(ACT_EN, HIGH);
      analogWrite(ACT_R_PWM, int(abs(actOutput)));
      analogWrite(ACT_L_PWM, 0);
    }
    else if (actInput < actSetpoint - actDeadband) {
      // FOR this setup, left turn is extending the actuator to turn ...
      digitalWrite(ACT_EN, HIGH);
      analogWrite(ACT_L_PWM, int(abs(actOutput)));
      analogWrite(ACT_R_PWM, 0);
    }

    // Set PWM for Tool
    if (toolInput <= toolSetpoint + toolDeadband && toolInput >= toolSetpoint - toolDeadband) {
      analogWrite(TOOL_R_PWM, 0);
      analogWrite(TOOL_L_PWM, 0);
    }
    else if (toolInput > toolSetpoint + toolDeadband) {
      digitalWrite(TOOL_EN, HIGH);
      analogWrite(TOOL_R_PWM, int(abs(toolOutput)));
      analogWrite(TOOL_L_PWM, 0);
    }
    else if (toolInput < toolSetpoint - toolDeadband) {
      // FOR this setup, left turn is extending the actuator to turn ...
      digitalWrite(TOOL_EN, HIGH);
      analogWrite(TOOL_L_PWM, int(abs(toolOutput)));
      analogWrite(TOOL_R_PWM, 0);
      // Serial.println(toolOutput);

    }
  }
}
