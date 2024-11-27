#include <ESP32Servo.h>

Servo servo;

void ServoSync(void *pvParameters) {
  servo.attach(33);
  
  delay(50);
  servo.write(90);

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
      continue;
    } else {
      delay(250);
    }
  }
}