// IR Sensor and LED Interaction Code
// Reads data from an IR sensor connected to pin 4 and controls an LED on pin 5

void setup() {
    pinMode(4, INPUT);  // IR Sensor input pin
    pinMode(5, OUTPUT); // LED output pin
    Serial.begin(9600); // Initialize Serial Monitor
}

void loop() {
    int irValue = digitalRead(4); // Read IR sensor value
    if (irValue == LOW) {
        digitalWrite(5, HIGH); // Turn LED on
    } else {
        digitalWrite(5, LOW);  // Turn LED off
    }
    Serial.println(irValue); // Print IR sensor value to Serial Monitor
    delay(10); // Short delay for stability
}
