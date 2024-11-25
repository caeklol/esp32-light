#include <ESP32Servo.h>

Servo servo;

void ServoSync(void *pvParameters) {
  servo.attach(33);
  
  delay(50);
  servo.write(90);

  for (;;) {
    if (lastState != state) {
      lastState = state;
      Serial.println(state);
      if (state) {
        servo.write(60);
      } else {
        servo.write(120);
      }
      continue;
    }

    if (!toggleSignal) {
      delay(100);
      continue;
    }

    if (lightState != adaptiveState) {
      adaptiveState = lightState;
    }
    
    adaptiveState = !adaptiveState;
    toggleSignal = false;
    Serial.print("Updating state:");
    Serial.println(adaptiveState);
    if (adaptiveState) {
      servo.write(60);
    } else {
      servo.write(120);
    }

    delay(500);
    servo.write(90);
  }
}