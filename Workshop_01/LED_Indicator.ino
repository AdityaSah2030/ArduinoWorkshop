void setup() {
  pinMode(11, OUTPUT);  // Set pin 11 as output
}

void loop() {
  // Turn LED on pin 11
  digitalWrite(11, 1);  
  
  delay(1000);  // Wait for 1 second

  // Turn LED off pin 11
  digitalWrite(11, 0);  
  
  delay(1000);  // Wait for 1 second
}
