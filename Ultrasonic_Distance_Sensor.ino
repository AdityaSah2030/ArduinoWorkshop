// Ultrasonic Distance Sensor Code
// Reads data from an ultrasonic distance sensor connected to pin 5 and prints the value to the Serial Monitor

void setup() {
    pinMode(5, INPUT);  // Sensor input pin
    Serial.begin(9600); // Initialize Serial Monitor
}

void loop() {
    int sensorValue = digitalRead(5); // Read sensor value
    Serial.println(sensorValue);      // Print value to Serial Monitor
    delay(1000); // Wait for 1 second before next reading
}
