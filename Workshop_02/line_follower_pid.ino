// Improved Adaptive PID Line Follower with Sharp Turn Recovery and U-Turn Handling

// Motor Pins
const int leftMotorIn1 = 4;
const int leftMotorIn2 = 5;
const int leftMotorEN = 6;
const int rightMotorIn1 = 7;
const int rightMotorIn2 = 8;
const int rightMotorEN = 9;

// 8-Channel IR Sensor Pins
const int sensorPins[8] = {2, 3, 10, 11, 12, A0, A1, A2};

// Sensor Weights (Center = 0, Left Negative, Right Positive)
const int weights[8] = {-350, -250, -150, -50, 50, 150, 250, 350};

// PID Variables
float Kp = 35;
float Ki = 0;
float Kd = 160;  // Fine-tuned for sharper handling

int lastError = 0;
float integral = 0;
unsigned long lastUpdateTime = 0;

// Base Speeds
int baseSpeed = 130;  // Set for straight lines
int minSpeed = 50;    // For tight turns

// Lost Line Recovery Variables
bool searching = false;
unsigned long lostTime = 0;

// Dead Zone
const int DEAD_ZONE = 5;

// Setup
void setup() {
    Serial.begin(9600);

    pinMode(leftMotorIn1, OUTPUT);
    pinMode(leftMotorIn2, OUTPUT);
    pinMode(leftMotorEN, OUTPUT);
    pinMode(rightMotorIn1, OUTPUT);
    pinMode(rightMotorIn2, OUTPUT);
    pinMode(rightMotorEN, OUTPUT);

    for (int i = 0; i < 8; i++) {
        pinMode(sensorPins[i], INPUT);
    }

    stopMotors();
    delay(1000);
}

// Main Loop
void loop() {
    int sensorValues[8];
    readSensors(sensorValues);

    int error = computeError(sensorValues);

    if (error == 999) {
        handleLostLine();
    } else {
        autoTunePID(error);
        float correction = computePID(error);
        adjustMotorSpeeds(correction, error);
        searching = false;  // Reset search mode if line is detected
    }

    delay(4);  // Small delay for stability
}

// Read Sensor Values
void readSensors(int sensorValues[]) {
    for (int i = 0; i < 8; i++) {
        sensorValues[i] = (digitalRead(sensorPins[i]) == LOW) ? 1 : 0;
    }
}

// Compute Error (returns 999 if no line detected)
int computeError(const int sensorValues[]) {
    long weightedSum = 0;
    int activeSensors = 0;

    for (int i = 0; i < 8; i++) {
        if (sensorValues[i] == 1) {
            weightedSum += weights[i];
            activeSensors++;
        }
    }

    if (activeSensors == 0) {
        return 999;  // Lost line signal
    }

    return weightedSum / activeSensors;
}

// PID Calculation
float computePID(int error) {
    float P = error;
    integral += error;  // Not used much (Ki = 0)
    float D = error - lastError;

    float output = Kp * P + Ki * integral + Kd * D;

    lastError = error;

    return output;
}

// Auto-tune Kp & Kd Based on Error
void autoTunePID(int error) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime > 100) {
        lastUpdateTime = currentTime;

        if (abs(error) > 300) {
            Kp = 45;  // Sharp turn - higher aggression
            Kd = 180; // More damping
        } else if (abs(error) > 150) {
            Kp = 40;  // Moderate turn
            Kd = 160;
        } else {
            Kp = 35;  // Straighter section
            Kd = 140;
        }
    }
}

// Adjust Motor Speeds
void adjustMotorSpeeds(float correction, int error) {
    int dynamicBaseSpeed = map(abs(error), 0, 350, baseSpeed, minSpeed + 20);
    dynamicBaseSpeed = constrain(dynamicBaseSpeed, minSpeed, baseSpeed);

    int leftSpeed, rightSpeed;

    if (abs(error) <= DEAD_ZONE) {
        leftSpeed = baseSpeed;  // Dead zone, go straight
        rightSpeed = baseSpeed;
    } else {
        leftSpeed = dynamicBaseSpeed + correction;
        rightSpeed = dynamicBaseSpeed - correction;
    }

    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    moveMotors(leftSpeed, rightSpeed);
}

// Move Motors with Direction Handling
void moveMotors(int leftSpeed, int rightSpeed) {
    if (leftSpeed >= 0) {
        digitalWrite(leftMotorIn1, HIGH);
        digitalWrite(leftMotorIn2, LOW);
    } else {
        digitalWrite(leftMotorIn1, LOW);
        digitalWrite(leftMotorIn2, HIGH);
        leftSpeed = -leftSpeed;
    }
    analogWrite(leftMotorEN, leftSpeed);

    if (rightSpeed >= 0) {
        digitalWrite(rightMotorIn1, HIGH);
        digitalWrite(rightMotorIn2, LOW);
    } else {
        digitalWrite(rightMotorIn1, LOW);
        digitalWrite(rightMotorIn2, HIGH);
        rightSpeed = -rightSpeed;
    }
    analogWrite(rightMotorEN, rightSpeed);
}

// Stop Motors
void stopMotors() {
    analogWrite(leftMotorEN, 0);
    analogWrite(rightMotorEN, 0);
    digitalWrite(leftMotorIn1, LOW);
    digitalWrite(leftMotorIn2, LOW);
    digitalWrite(rightMotorIn1, LOW);
    digitalWrite(rightMotorIn2, LOW);
}

// Lost Line Handling (Recovery + U-Turn Handling)
void handleLostLine() {
    if (!searching) {
        searching = true;
        lostTime = millis();
        stopMotors();
        delay(50);  // Brief pause before recovery
    }

    unsigned long searchDuration = millis() - lostTime;

    if (searchDuration < 400) {
        if (lastError < 0) {
            moveMotors(-70, 70);  // Gentle left spin (recover left side loss)
        } else {
            moveMotors(70, -70);  // Gentle right spin
        }
    } else {
        // After 400ms, increase spin speed (assume U-turn needed)
        if (lastError < 0) {
            moveMotors(-100, 100);  // Aggressive left spin
        } else {
            moveMotors(100, -100);  // Aggressive right spin
        }
    }
}
