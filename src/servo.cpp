#include "servo.h"

Servo servo;

bool lastState = true;
bool state = false;

void ServoSync(void *pvParameters) {
  servo.attach(33);
  delay(50);
  servo.write(90);
  delay(200);
  for (;;) {
    if (lastState != state) {
      Serial.println("Syncing state");

      if (state) {
        servo.write(60);
      } else {
        servo.write(120);
      }

      lastState = state;

      delay(100);
      servo.write(90);
    } else {
      delay(250);
    }
  }
}
