#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>

struct WiFiCredentials {
  String ssid;
  String password;
};

typedef WiFiCredentials WiFiCredentials;

void setLED(bool state);
void connectToWiFi(const String& ssid, const String& password);
WiFiCredentials loadCreds();
void WifiServer(void *pvParameters);

extern WebServer server;
extern Preferences preferences;

#endif
