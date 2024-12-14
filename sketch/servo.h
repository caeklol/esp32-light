#ifndef SERVOSYNCHANDLER_H
#define SERVOSYNCHANDLER_H

#include <Arduino.h>
#include <ESP32Servo.h>

extern Servo servo;
extern bool state;
extern bool lastState;

void ServoSync(void *pvParameters);

#endif 