#include <Arduino.h>
#include <Preferences.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

#include "wifi.h"
#include "server.h"

char* defaultSSID = "defaultSSID";  
char* defaultPassword = "password";

WebServer server(80);
Preferences preferences;

void setLED(bool state) {
  digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
}

void connectToWiFi(const String& ssid, const String& password) {
  WiFi.begin(ssid.c_str(), password.c_str());

  setLED(false);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("#");
    delay(300);
    setLED(!digitalRead(LED_BUILTIN));
  }
  
  Serial.println();
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp32")) {
    Serial.println("Error setting up mDNS responder");
    return;
  }

  Serial.println("mDNS responder started");
  server.begin();
  Serial.println("Web server started");

  MDNS.addService("http", "tcp", 80);
  setLED(true);
}

WiFiCredentials loadCreds() {
  WiFiCredentials creds;
  creds.ssid = preferences.getString("ssid", "");
  creds.password = preferences.getString("password", "");
  
  if (creds.ssid.isEmpty()) {
    creds.ssid = defaultSSID;
    creds.password = defaultPassword;
  }

  return creds;
}

void WifiServer(void *pvParameters) {
  Serial.println("Loading preferences...");
  preferences.begin("esp32-light", false);

  WiFiCredentials creds = loadCreds();

  while (creds.ssid.isEmpty()) {
    Serial.println("Credentials are empty, waiting for recovery..."); // TODO
    delay(10000);
  }

  Serial.println("Connecting...");  
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      creds = loadCreds();
      connectToWiFi(creds.ssid, creds.password);
    }

    setupServerEndpoints();
  }
}
