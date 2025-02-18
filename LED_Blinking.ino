// LED Blinking Code
// This code alternates the blinking of two LEDs connected to pins 11 and 8

void setup() {
    pinMode(11, OUTPUT); // LED 1
    pinMode(8, OUTPUT);  // LED 2
}

void loop() {
    // Turn both LEDs ON
    digitalWrite(11, HIGH);
    digitalWrite(8, HIGH);
    delay(1000); // Wait for 1 second

    // Turn both LEDs OFF
    digitalWrite(11, LOW);
    digitalWrite(8, LOW);
    delay(1000); // Wait for 1 second
}
