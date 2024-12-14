#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

const char* ssid = "credentials";  
const char* password = "credentials";

WiFiServer server(80);

struct WiFiCredentials {
  String ssid;
  String password;
};

typedef WiFiCredentials WiFiCredentials;

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
  Serial.println("TCP server started");

  MDNS.addService("http", "tcp", 80);
  setLED(true);
}

void handleIncoming() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("New client");

  while (client.connected() && !client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println(req);
    return;
  }

  req = req.substring(addr_start + 1, addr_end);
  
  if (req == "/toggle") {
    state = !state;
    client.println(String(state));
    client.println();
  }

  client.println("HTTP/1.1 200 OK\r\n\r\n");
  client.stop();
  Serial.println("Response sent");
}

struct WiFiCredentials loadCreds() {
  WiFiCredentials creds;
  creds.ssid = preferences.getString("ssid", "");
  creds.password = preferences.getString("password", "");
  
  if (creds.ssid.isEmpty()) {
    creds.ssid = "";
    creds.password = "";
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

    handleIncoming();
  }
}
