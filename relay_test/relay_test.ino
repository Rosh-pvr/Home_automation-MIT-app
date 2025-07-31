#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

const char* ssid = "GRS_NET";     // Replace with your WiFi SSID
const char* password = "roshan18"; // Replace with your WiFi password

WebServer server(80);

const int relayPin = 13; // GPIO pin connected to relay
bool relayState = false;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ensure relay is off initially

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup server routes
  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/status", handleStatus);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP-32 Relay Control</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>body{font-family:Arial,sans-serif;text-align:center;margin-top:50px;}";
  html += "button{padding:10px 20px;font-size:16px;margin:10px;}</style></head><body>";
  html += "<h1>Relay Control</h1>";
  html += "<p>Current State: " + String(relayState ? "ON" : "OFF") + "</p>";
  html += "<a href=\"/on\"><button>Turn ON</button></a>";
  html += "<a href=\"/off\"><button>Turn OFF</button></a>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleOn() {
  digitalWrite(relayPin, HIGH);
  relayState = true;
  server.send(200, "text/plain", "Relay turned ON");
}

void handleOff() {
  digitalWrite(relayPin, LOW);
  relayState = false;
  server.send(200, "text/plain", "Relay turned OFF");
}

void handleStatus() {
  server.send(200, "text/plain", relayState ? "ON" : "OFF");
}

void handleNotFound() {
  server.send(404, "text/plain", "Not Found");
}