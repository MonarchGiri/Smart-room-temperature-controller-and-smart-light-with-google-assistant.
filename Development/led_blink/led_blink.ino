int Led = 12; // setting the IO pin for LED

void setup() {
  pinMode(Led, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void loop() {
  digitalWrite(Led, HIGH);  // Turn the LED off by making the voltage HIGH
  
}
