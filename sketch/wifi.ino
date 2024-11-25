#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>

const char* ssid = "credentials";  
const char* password = "credentials";

WiFiServer server(80);

void initWiFiServer() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(300);
    digitalWrite(LED_BUILTIN, LOW);
    delay(300);
  }
  
  Serial.println();
  Serial.print("ip:");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp32")) {
    Serial.println("Error setting up MDNS responder!");
    showFailure();
  }

  Serial.println("mDNS responder started");

  server.begin();
  Serial.println("TCP server started");

  MDNS.addService("http", "tcp", 80);
}

void handleIncoming() {
  WiFiClient client = server.available();
  if (!client) {
        return;
    }

    Serial.println("");
    Serial.println("New client");

    while(client.connected() && !client.available()){
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
    } else if (req == "/adaptivetoggle") {
      toggleSignal = true;
    }

    Serial.println("Done parsing");
    client.println("HTTP/1.1 200 OK\r\n\r\n");
    client.stop();
    Serial.println("Sent response");
}

void WifiServer(void *pvParameters){
  initWiFiServer();

  for (;;) {
    handleIncoming();
  }
}
