#ifndef SAFETY_H
#define SAFETY_H
#include <Arduino.h>

extern bool error;
extern bool enable;
extern String errorMessage;

void startSafety();
void checkEstop();

#endif
