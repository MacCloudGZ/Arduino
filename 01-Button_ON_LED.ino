// set pin numbers
const int buttonPin = 14; // the number of the pushbutton pinconst 
int ledPin = 27; // the number of the LED pin

// variable for storing the pushbutton status
int buttonState = 0;
void setup() {
    Serial.begin(115200);
    pinMode(buttonPin, INPUT); // initialize the pushbutton pinasaninput
    pinMode(ledPin, OUTPUT); // initialize the LED pin as an output
    }
void loop() {
    // read the state of the pushbutton value
    buttonState = digitalRead(buttonPin);
    Serial.println(buttonState);
    // check if the pushbutton is pressed.
    // if it is, the buttonState is HIGH
    if (buttonState == HIGH) {
    // turn LED on
    digitalWrite(ledPin, HIGH);
    } else {
    // turn LED off
    digitalWrite(ledPin, LOW);
    }
}
