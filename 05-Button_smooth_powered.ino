const int buttonPin = 27; // Change this to your button GPIO
const int ledPin = 14;

// PWM properties
const int freq = 5000;
const int resolution = 8; 

int dutyCycle = 0;
int buttonState = 0;

void setup() {
    Serial.begin(115200);
    // Use INPUT_PULLUP so you don't need an external resistor.
    // Connect button between Pin 4 and GND.
    pinMode(buttonPin, INPUT_PULLUP);
    
    // Setup PWM (ESP32 Core v3.0+)
    ledcAttach(ledPin, freq, resolution);
}

void loop() {
    // Read button (LOW = Pressed because of Pullup)
    buttonState = digitalRead(buttonPin);

    // If button is PRESSED (LOW)
    if (buttonState == HIGH) {
        // Increase brightness, but stop at 255
        if (dutyCycle < 255) {
            dutyCycle++;
        }
    } 
    // If button is RELEASED (HIGH)
    else {
        // Decrease brightness, but stop at 0
        if (dutyCycle > 0) {
            dutyCycle--;
        }
    }

    // Apply the brightness
    ledcWrite(ledPin, dutyCycle);
    
    // Debugging
    Serial.print("Button: ");
    Serial.print(buttonState);
    Serial.print(" | PWM: ");
    Serial.println(dutyCycle);

    // Small delay to make the fade visible (smoothness)
    delay(10); 
}