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
  html += "<meta charset='UTF-8'>";
  html += "<title>Light Debug</title>";
  html += "<link rel='preconnect' href='https://fonts.googleapis.com'>";
  html += "<link rel='preconnect' href='https://fonts.gstatic.com' crossorigin>";
  html += "<link href='https://fonts.googleapis.com/css2?family=Inter:ital,opsz,wght@0,14..32,100..900;1,14..32,100..900&display=swap' rel='stylesheet'>";
  html += "<style>";
  html += "body { font-size: 1em; text-align:center; background-color: #11111b; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; color: #e9ecf5; font-family: Inter, sans-serif;}";
  html += ".container { border-radius: 5px; padding: 20px; padding-right: 40px; padding-left: 40px; background-color: #1e1e2e; }";
  html += "</style>";
  html += "</head>";
  html += "<body><div class='container'>";
  html += content;
  html += "</div></body>";
  html += "</html>";
  return html;
}

void handleHome() {
  String content = "<h1>hello!</h1>";
  content += "<div style='font-size:48px;animation:bounce 1s infinite;'>&#128075;</div>";
  content += "<div class='container'>";
  content += "<style>";
  content += "button { background-color: #e5c890; color: #292c3c; border-radius: 5px; border-width: 0; padding: 5px; }";
  content += "button: active { background-color: #f0c369; }";
  content += ".green { color: #a6d189; }";
  content += ".red { color: #e78284; }";
  content += "</style>";
  content += "<h2>Light is currently</h2>";
  content += "<p><strong";
  content += state ? "class='green'>ON" : "class='red'>OFF";
  content += "</strong></p>";
  content += "<form action='/toggle' method='GET'><button type='submit'>Toggle the lights</button></form>";
  content += "</div>";
  content += "<style>@keyframes bounce{0%,100%{transform:translateY(10px);}50%{transform:translateY(-10px);}}</style>";
  server.send(200, "text/html", getPage(content));
}

void handleToggle() {
  state = !state;
  String content = "<strong>successful! redirecting...</strong>";
  content += "<br>";
  content += "<br>";
  content += "<br>";
  content += "<i font-size: 0.1em;>id: ";
  content += "<meta http-equiv='refresh' content='0; url=/'>";
  content += "</i>";
  
  server.send(200, "text/html", getPage(content));
}

void handleState() {
	HTTPMethod method = server.method();
	if (method == HTTP_POST) {
		if (server.hasArg("state")) {
			bool newState = server.arg("state") == "true";
			state = newState;
		} else {
			server.send(500, "text/plain", "State not provided");
		}
	}
	server.send(200, "text/plain", state ? "true" : "false");
}

void setupServerEndpoints() {
  Serial.println("Setting up endpoints");
  server.on("/", handleHome);
  server.on("/toggle", handleToggle);
  server.on("/state", handleState);
  server.begin();
}
