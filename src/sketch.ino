#include <Arduino.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//#include <server.h>
#include <wifi.h>
#include <servo.h>

void loop() {}

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
    ,  8196
    ,  NULL
    ,  2
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
