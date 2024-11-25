#include <Preferences.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

bool lightState = true;
bool adaptiveState = true;

bool toggleSignal = false;

bool state = true;
bool lastState = false;

bool error = false;

void showFailure() {
  error = true;
  for(;;) { delay(10); }
}

void loop() {
  if (!error) {
    delay(10);
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}

void disableBrownout() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
}

void initRtos() {
  Serial.println("Initializing tasks...");

  xTaskCreate(
    ServoSync
    ,  "Servo Sync"
    ,  2048
    ,  NULL
    ,  1
    ,  NULL
  );

  xTaskCreate(
    WifiServer
    ,  "Wifi Server"
    ,  2048
    ,  NULL
    ,  2
    ,  NULL
  );

  xTaskCreate(
    StateSync
    ,  "State Sync"
    ,  2048
    ,  NULL
    ,  3
    ,  NULL
  );
  
  Serial.println("Done!");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello!");

  pinMode(LED_BUILTIN, OUTPUT);

  disableBrownout();

  initRtos();
}