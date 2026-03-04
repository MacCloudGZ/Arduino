#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Hello";
const char* password = "00000000";

WebServer server(80);

String latitude = "Not received";
String longitude = "Not received";

void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>ESP32 GPS Server</title></head>";
  html += "<body>";
  html += "<h2>ESP32 GPS Receiver</h2>";
  html += "<p><b>Latitude:</b> " + latitude + "</p>";
  html += "<p><b>Longitude:</b> " + longitude + "</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleGPS() {
  if (server.hasArg("lat") && server.hasArg("lon")) {
    latitude = server.arg("lat");
    longitude = server.arg("lon");
    Serial.println("Received GPS:");
    Serial.println(latitude + ", " + longitude);
    server.send(200, "text/plain", "GPS Received");
  } else {
    server.send(400, "text/plain", "Missing Data");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/gps", handleGPS);

  server.begin();
}

void loop() {
  server.handleClient();
}