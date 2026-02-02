// the number of the LED pin
const int ledPin = 14; // 4 corresponds to GPIO4
// setting PWM properties
const int freq = 5000;
const int resolution = 8;
void setup(){
  // configure LED PWM functionalitites
  ledcAttach(ledPin, freq, resolution);
  // attach the channel to the GPIO to be controlled
}
void loop(){
  // increase the LED brightness
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){
    // changing the LED brightness with PWM
    ledcWrite(ledPin, dutyCycle);
    delay(15);
  }
  // decrease the LED brightness
  delay(2000);
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(ledPin, dutyCycle);
    delay(15);
  }
}
