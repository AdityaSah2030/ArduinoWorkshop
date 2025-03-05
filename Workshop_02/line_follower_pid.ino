// Line Follower Robot with PID Control (8-Channel IR Sensor)

// Motor Driver Pin Definitions (L298N)
const int leftMotorIn1 = 4;
const int leftMotorIn2 = 5;
const int leftMotorEN = 6;

const int rightMotorIn1 = 7;
const int rightMotorIn2 = 8;
const int rightMotorEN = 9;

// 8-Channel IR Sensor Pin Definitions
const int sensorPins[8] = {2, 3, 10, 11, 12, A0, A1, A2};

// Sensor Weights - Center is 0, left is negative, right is positive
const int weights[8] = {-350, -250, -150, -50, 50, 150, 250, 350};

// PID Control Constants (Tunable Parameters)
float Kp = 50;    // Proportional Gain
float Ki = 0;     // Integral Gain (often minimal for line followers)
float Kd = 150;   // Derivative Gain (reduced from 300 for noise reduction)

// PID Variables
int lastError = 0;
float integral = 0;

// Speed Control Parameters
int baseSpeed = 70;  // Default speed on straight paths
int minSpeed = 40;   // Minimum speed during sharp turns

void setup() {
    Serial.begin(9600);
    
    // Configure Motor Driver Pins as Outputs
    pinMode(leftMotorIn1, OUTPUT);
    pinMode(leftMotorIn2, OUTPUT);
    pinMode(leftMotorEN, OUTPUT);
    pinMode(rightMotorIn1, OUTPUT);
    pinMode(rightMotorIn2, OUTPUT);
    pinMode(rightMotorEN, OUTPUT);
    
    // Configure Sensor Pins as Inputs
    for (int i = 0; i < 8; i++) {
        pinMode(sensorPins[i], INPUT);
    }
    
    stopMotors();
    delay(1000); // Allow time for sensor stabilization
}

void loop() {
    int sensorValues[8];
    readSensors(sensorValues);
    
    int error = computeError(sensorValues);
    float correction = computePID(error);
    
    adjustMotorSpeeds(correction, error);
    
    delay(6);  // Short delay for stability
}

// Read sensor values (1 if detecting black line, 0 otherwise)
void readSensors(int sensorValues[]) {
    for (int i = 0; i < 8; i++) {
        int reading = digitalRead(sensorPins[i]);
        sensorValues[i] = (reading == LOW) ? 1 : 0;
    }
}

// Compute positional error based on sensor readings
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
        stopMotors();
        integral = 0;
        lastError = 0;
        return 0; // No line detected - Stop or handle recovery
    }
    
    int error = weightedSum / activeSensors;
    
    // Apply a deadband to filter minor fluctuations
    if (abs(error) < 10) {
        error = 0;
    }
    
    return error;
}

// Compute PID correction based on error
float computePID(int error) {
    float P = error;
    integral += error;
    float D = error - lastError;

    float output = Kp * P + Ki * integral + Kd * D;

    lastError = error;
    return output;
}

// Adjust motor speeds based on PID correction
void adjustMotorSpeeds(float correction, int error) {
    // Adjust base speed dynamically based on error magnitude
    int dynamicBaseSpeed = map(abs(error), 0, 350, baseSpeed, minSpeed);
    dynamicBaseSpeed = constrain(dynamicBaseSpeed, minSpeed, baseSpeed);

    int leftSpeed = dynamicBaseSpeed + correction;
    int rightSpeed = dynamicBaseSpeed - correction;

    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    moveMotors(leftSpeed, rightSpeed);
}

// Control motor movements based on calculated speeds
void moveMotors(int leftSpeed, int rightSpeed) {
    // Left Motor Control
    if (leftSpeed >= 0) {
        digitalWrite(leftMotorIn1, HIGH);
        digitalWrite(leftMotorIn2, LOW);
    } else {
        digitalWrite(leftMotorIn1, LOW);
        digitalWrite(leftMotorIn2, HIGH);
        leftSpeed = -leftSpeed;  // Convert to positive for PWM
    }
    analogWrite(leftMotorEN, leftSpeed);

    // Right Motor Control
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

// Stop both motors
void stopMotors() {
    analogWrite(leftMotorEN, 0);
    analogWrite(rightMotorEN, 0);
    digitalWrite(leftMotorIn1, LOW);
    digitalWrite(leftMotorIn2, LOW);
    digitalWrite(rightMotorIn1, LOW);
    digitalWrite(rightMotorIn2, LOW);
}
