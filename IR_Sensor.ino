// IR Sensor Code
// Reads data from an IR sensor connected to pin 4 and prints 1 if an object (e.g., a hand) is detected, otherwise prints 0

void setup() {
    pinMode(4, INPUT);  // IR Sensor input pin
    Serial.begin(9600); // Initialize Serial Monitor
}

void loop() {
    int irValue = digitalRead(4); // Read IR sensor value
    if (irValue == LOW) {
        Serial.println(1); // Object detected
    } else {
        Serial.println(0); // No object detected
    }
    delay(10); // Short delay for stability
}
