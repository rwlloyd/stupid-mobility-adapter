#include <Ps3Controller.h>

#include "safety.h"
#include "config.h"
//#include "dc_motor.h"
#include "rcinput.h"
#include "curtis.h"
#include "actuator.h"
#include "comms.h"

bool debugController = false;
bool debugSetpoints = false;
bool debugCurtis = false;
bool debugActuator = false;
bool debugServoInput = false;
bool commandWaiting = false;

String esp_address; //variable to save the esp bluetooth address useful for debugging.
//int motorSpeed;

// P3s controller output minimums and maximums.
// Minimums are down and left on analog sticks
// Order: left_x, left_y, left_trigger_2, right_x, right_y, right_trigger_2
int left_x, left_y, left_trigger_2, right_x, right_y, right_trigger_2;
int ps3Mins[] = { -128, -128, 0,  -128, 128,  0};
int ps3Maxs[] = {128,  128,  255, 128, -128, 255};
// Just so we're not trying to do everything with minimal input, we'll have a deazone
int deadzone = 12;
// if we want to check the battery on the controller
int battery = 0;

void onConnect() {
  Serial.println("Controller Connected.");
}

void onDisconnect() {
  Serial.println("Controller Disconnected");
  error = true;
  errorMessage = "4 Controller Connection Lost.";
  motorSpeed = 0;

}

void getStates() {
  // Callback function to look at the last messages from the ps3 controller

  if ( Ps3.event.button_down.start ) {
    // Serial.println("Started pressing the triangle start");
    enableServoInput = !enableServoInput;
  }
  // If we press the 'triangle' button...
  if ( Ps3.event.button_down.triangle ) {
    // Serial.println("Started pressing the triangle button");
    enable = !enable;
  }
  if ( Ps3.event.button_down.circle ) {
    // Serial.println("Started pressing the circle button");
    debugActuator = !debugActuator;
  }
  if ( Ps3.event.button_down.square ) {
    // Serial.println("Started pressing the square button");
    debugController = !debugController;
  }

  if ( Ps3.event.button_down.cross ) {
    // Serial.println("Started pressing the cross button");
    debugSetpoints = !debugSetpoints;
    debugCurtis = !debugCurtis;
  }

  // Get the inputs from the controller and make them all 0-255 (mapped)
  left_x = Ps3.data.analog.stick.lx; //left=-128 right=128
  left_y = Ps3.data.analog.stick.ly; // up=128 down=-128
  left_trigger_2 = Ps3.data.analog.button.l2;  // 0-255

  right_x = Ps3.data.analog.stick.rx;//left=-128 right=128
  right_y = Ps3.data.analog.stick.ry; // up=-128 down=128
  right_trigger_2 = Ps3.data.analog.button.r2; // 0 -> 255

  if (enableServoInput) {
    if (!error) {
      //doRCInput();
      processSerialCommand();
    }
  } else { // get input from the bt joysticks
    // forward/back control
    // left_y up=128 down=-128
    if (abs(left_y) > deadzone) {
      motorSpeed = left_y;
    } else {
      motorSpeed = 0;
    }

    //right_y left=-128 right=128
    if (abs(right_x) > deadzone) {
      actuatorSetpoint_request = right_x;
    } else {
      actuatorSetpoint_request = 0;
    }

    if (abs(left_trigger_2) > 200){
      toolSetpoint_request = toolMin;
    }
    if (abs(right_trigger_2) > 200){
      toolSetpoint_request = toolMax;
    }

  }

  if (debugController) {
    Serial.print(left_x); Serial.print(", ");       Serial.print(left_y); Serial.print(", ");
    Serial.print(left_trigger_2); Serial.print(", "); Serial.print(right_x); Serial.print(", ");
    Serial.print(right_y); Serial.print(", ");       Serial.println(right_trigger_2);
  }

  if (debugSetpoints) {
#ifdef ALL_BTS7960
    Serial.print(motorSpeed); Serial.print(", "); Serial.println(actuatorSetpoint_request);
#endif

#ifdef CURTIS
    Serial.print(motorSpeed); Serial.print(", "); Serial.print(actuatorSetpoint_request); Serial.print(", ");
#endif
  }

  //---------------------- Battery events ---------------------
  if ( battery != Ps3.data.status.battery ) {
    battery = Ps3.data.status.battery;
    Serial.print("The controller battery is ");
    if ( battery == ps3_status_battery_charging )      Serial.println("charging");
    else if ( battery == ps3_status_battery_full )     Serial.println("FULL");
    else if ( battery == ps3_status_battery_high )     Serial.println("HIGH");
    else if ( battery == ps3_status_battery_low)       Serial.println("LOW");
    else if ( battery == ps3_status_battery_dying )    Serial.println("DYING");
    else if ( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
    else Serial.println("UNDEFINED");
  }
}

void setupPs3() {
  // PS3 controller setup
  Ps3.begin(); // instance of the ps3 controller
  esp_address = Ps3.getAddress(); // save the mac address
  Ps3.attach(getStates);
  Ps3.attachOnConnect(onConnect);
  Ps3.attachOnDisconnect(onDisconnect);
  //  Ps3.begin("FC:F5:C4:00:Fa:C6");  // MAC address of the esp32 //HWPROTOTYPE
  //Ps3.begin("bt:24:d7:eb:0e:df:06");  // DeskTesting
  Ps3.begin("94:b5:55:2b:b4:1a");  // NEwESP
  if (!Ps3.isConnected()) {
    Serial.println("Controller not yet connected");
  } else {
    Serial.println("Controller Ready.");
  }
  Serial.print("ESP32 Address: ");
  Serial.println(esp_address);
}

void checkPs3() {
  if (!Ps3.isConnected()) {
    // Serial.println("Controller not connected");
    error = true;
    errorMessage = "3 Bluetooth Not Connected";
    return; //if we're not connected yet, do nothing
  }
}
