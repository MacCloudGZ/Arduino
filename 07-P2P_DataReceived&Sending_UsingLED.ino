#include <esp_now.h>
#include <WiFi.h>

const int buttonPin = 27; 
const int ledPin = 14;

// Use the MAC address of the TARGET ESP32
uint8_t peerAddress[] = {0xB0, 0xCB, 0xD8, 0x89, 0xC8, 0x30};

typedef struct struct_message {
    int btnState;
} struct_message;

struct_message myData;
struct_message incomingReadings;

int lastButtonState = LOW; // For edge detection

// Updated Callback for ESP32 Core 3.0+ 
void OnDataRecv(const esp_now_recv_info_t * recv_info, const uint8_t *incomingData, int len) {
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  digitalWrite(ledPin, incomingReadings.btnState);
  Serial.print("Received state: ");
  Serial.println(incomingReadings.btnState);
  delay(200);
  digitalWrite(ledPin, 0);
}

void setup() {
  Serial.begin(115200); 
  
  // Using INPUT_PULLUP is safer if your button connects to GND
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT); 

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  esp_now_peer_info_t peerInfo = {}; // Initialize with zeros
  memcpy(peerInfo.peer_addr, peerAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // 1. Read button (Inverted logic because of INPUT_PULLUP)
  int currentButtonState = !digitalRead(buttonPin); 
  
  // 2. Only send if the state has CHANGED (Edge Detection)
  if (currentButtonState != lastButtonState) {
    myData.btnState = currentButtonState;
    esp_now_send(peerAddress, (uint8_t *) &myData, sizeof(myData));
    
    Serial.print("Sent state: ");
    Serial.println(currentButtonState);
    
    lastButtonState = currentButtonState;
  }

  delay(10); // Faster polling but less network traffic
}