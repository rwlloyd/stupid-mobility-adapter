#ifndef SAFETY_H
#define SAFETY_H
#include <Arduino.h>

extern bool error;
extern bool enable;
extern String errorMessage;
extern bool enableServoInput;

extern int batteryVoltage;

void startSafety();
void checkEstop();
void checkBattery();
void doSafety();

#endif
