#include <WiFi.h>
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
}
void loop(){
  Serial.print("ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
}
