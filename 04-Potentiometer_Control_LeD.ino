// Potentiometer is connected to GPIO 27 (ADC2)
const int potPin = 27;
const int ledPin = 14; 

// PWM properties
const int freq = 5000;
const int resolution = 8; // 8-bit means values 0-255

// variable for storing the potentiometer value
int potValue = 0;
int pwmValue = 0;

void setup() {
  Serial.begin(9600);
  // Configure LED PWM (ESP32 Core v3.0+ syntax)
  ledcAttach(ledPin, freq, resolution);
  delay(1000);
}

void loop() {
  // 1. Read potentiometer value (Range: 0 - 4095)
  potValue = analogRead(potPin);

  // 2. Map the value to fit the PWM range (Range: 0 - 255)
  // map(value, fromLow, fromHigh, toLow, toHigh)
  pwmValue = map(potValue, 0, 4095, 0, 255);

  // 3. Write the mapped value to the LED
  ledcWrite(ledPin, pwmValue); // Added semicolon here

  // Debugging: Print both values to see the conversion
  Serial.print("Pot: ");
  Serial.print(potValue);
  Serial.print(" | PWM: ");
  Serial.println(pwmValue);

  delay(15);
}z