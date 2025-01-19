#include "servo.h"

Servo servo;

bool lastState = true;
bool state = false;

uint8_t counter = 0;

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
    } else {
      delay(250);
    }
	
	// ever since the inception of this project and concept i have had issues with reliability
	// of the servo. i initially thought this was a power supply issue, as typically power cycling
	// fixed the problem. i split the power supply in the board design to an external 5v 3a supply
	// connected to the servo directly, with ESP32 being powered by wall USB. no change.
	//
	// then i thought, what if it isn't the power supply? what if it's the actual signal? PWM is a
	// timing-based thing so perhaps the long lack of signal from the arduino causes the servo to
	// desync or smt. so no; this code is NOT useless.
	counter++;
	if (counter % 2 == 0) {
		servo.write(92);
	} else {
		servo.write(90);
	}
  }
}
