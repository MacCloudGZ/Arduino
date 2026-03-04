#include <WiFi.h>
#include <TinyGPS++.h> // Library to parse NMEA data

WiFiServer server(80);             // HTTP server on Port 80
TinyGPSPlus gps;                     // GPS parser
HardwareSerial SerialGPS(2); // Use Serial2 (Hardware Serial) [cite: 13, 16]


void setup() {
      Serial.begin(115200);
  // Initialize Hardware Serial for GPS (Baud, Mode, RX, TX) [cite: 16]
     SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
   }

void loop() {
  // Constantly read from GPS module and parse data
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
    if (gps.location.isValid()) {
      Serial.println(String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6));
    } else {
      Serial.println("Searching...");
          }
    delay(100);
  }        
}

