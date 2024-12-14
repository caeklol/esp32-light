#include <Arduino.h>
#include <Preferences.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

#include "wifi.h"
#include "server.h"
#include "servo.h"

// credit: https://github.com/ESP32-Work/ESP32_Webserver/blob/main/ESP32_WebServer/src/main.cpp

String getPage(String content) {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Light Debug</title>";
  html += "<style>";
  html += "body { background-color: black; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; color: white; }";
  html += ".container { padding: 20px; border: 2px solid white; }";
  html += "</style>";
  html += "</head>";
  html += "<body><div class='container'>";
  html += content;
  html += "</div></body>";
  html += "</html>";
  return html;
}

void handleHome() {
  String content = "<h1>hello</h1>";
  content += "<div style='text-align:center;'>";
  content += "<div style='font-size:48px;animation:bounce 1s infinite;'>👋</div>";
  content += "<div style='background:#e8f5e9;padding:10px;border-radius:5px;margin-top:20px;'>";
  content += "<h2>Light is currently</h2>";
  content += "<p><strong>";
  content += state ? "ON" : "OFF";
  content += "</strong></p>";
  content += "</div>";
  content += "</div>";
  content += "<style>@keyframes bounce{0%,100%{transform:translateY(0);}50%{transform:translateY(-20px);}}</style>";
  server.send(200, "text/html", getPage(content));
}

void handleToggle() {
  state = !state;
  server.send(200, "text/plain", String(state));
}

void setupServerEndpoints() {
  server.on("/", handleHome);
  server.on("/toggle", handleToggle);
  // server.on("/prefs", handleEdit);
  server.begin();
}
