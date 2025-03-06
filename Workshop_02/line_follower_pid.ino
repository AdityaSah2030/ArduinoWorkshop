// Master Adaptive PID Line Follower with Smooth Turns,
// T-Junction LED & U-Turn Handling

// ===================
// Motor Driver Pins (L298N)
// ===================
const int leftMotorIn1 = 4;
const int leftMotorIn2 = 5;
const int leftMotorEN   = 6;
const int rightMotorIn1 = 7;
const int rightMotorIn2 = 8;
const int rightMotorEN  = 9;

// ===================
// 8-Channel IR Sensor Pins
// ===================
const int sensorPins[8] = {2, 3, 10, 11, 12, A0, A1, A2};

// ===================
// Sensor Weights for error calculation
// ===================
// The weights determine the influence of each sensor on the overall error.
const int weights[8] = {-350, -250, -150, -50, 50, 150, 250, 350};

// ===================
// PID Control Variables
// ===================
float Kp = 12;    // Proportional constant
float Ki = 0;     // Integral constant (not used)
float Kd = 36;    // Derivative constant

int lastError = 0;   // Previous error, used for derivative calculation
float integral = 0;  // Accumulated error (unused since Ki=0)

// ===================
// Speed Settings
// ===================
int baseSpeed = 90;  // Base speed for motors
int minSpeed  = 40;  // Minimum motor speed
int maxSpeed  = 120; // Maximum motor speed for straights if needed

// ===================
// Dead Zone for Center
// ===================
const int DEAD_ZONE = 5;  // Unused variable in the current code, but kept for potential future use

// ===================
// T-Junction LED Pin
// ===================
const int ledPin = 13;  // LED to indicate T-Junction detection

// ===================
// Track Loss Recovery Variables
// ===================
bool searching = false;         // Flag to indicate if the robot is in search mode
unsigned long lostTime = 0;     // Timestamp when the line was lost

// ===================
// Setup Function: Initializes pins and motors
// ===================
void setup() {
    Serial.begin(9600);

    // Set motor driver pins as outputs
    pinMode(leftMotorIn1, OUTPUT);
    pinMode(leftMotorIn2, OUTPUT);
    pinMode(leftMotorEN, OUTPUT);
    pinMode(rightMotorIn1, OUTPUT);
    pinMode(rightMotorIn2, OUTPUT);
    pinMode(rightMotorEN, OUTPUT);

    // Set the T-Junction LED pin as output
    pinMode(ledPin, OUTPUT);

    // Set IR sensor pins as inputs
    for (int i = 0; i < 8; i++) {
        pinMode(sensorPins[i], INPUT);
    }

    // Ensure motors are stopped before starting
    stopMotors();
    delay(1000);
}

// ===================
// Main Loop: Reads sensor values, detects junctions, and applies PID control
// ===================
void loop() {
    int sensorValues[8];
    readSensors(sensorValues); // Read current state of IR sensors

    // Check for T-Junction using sensor readings
    if (detectTJunction(sensorValues)) {
        digitalWrite(ledPin, LOW); // LED off indicates T-Junction detected
    } else {
        digitalWrite(ledPin, HIGH); // LED on otherwise
    }

    // Calculate the error based on sensor values
    int error = computeError(sensorValues);

    // If error equals 999, line is lost; handle recovery
    if (error == 999) {
        handleLostLine();
    } else {
        adaptivePIDTuning(error);             // Adjust PID constants based on error magnitude
        float correction = computePID(error);   // Compute PID correction value
        adjustMotorSpeedsSmooth(correction, error); // Adjust motor speeds smoothly based on PID correction
        searching = false;                      // Reset searching flag since line is detected
    }

    delay(4); // Short delay to stabilize loop timing
}

// ===================
// Sensor Reading Function: Reads all 8 IR sensors
// ===================
void readSensors(int sensorValues[]) {
    for (int i = 0; i < 8; i++) {
        // Digital read: sensor returns LOW when line is detected
        sensorValues[i] = digitalRead(sensorPins[i]) == LOW ? 1 : 0;
    }
}

// ===================
// Error Calculation: Computes weighted average error from sensor readings
// ===================
int computeError(const int sensorValues[]) {
    long weightedSum = 0;
    int activeSensors = 0;

    // Sum up the weighted contributions of each active sensor
    for (int i = 0; i < 8; i++) {
        if (sensorValues[i] == 1) {
            weightedSum += weights[i];
            activeSensors++;
        }
    }

    // If no sensors are active, the line is lost
    if (activeSensors == 0) {
        return 999;  // Special error value indicating line loss
    }

    // Return the average weighted error
    return weightedSum / activeSensors;
}

// ===================
// PID Controller: Computes the correction value using the PID algorithm
// ===================
float computePID(int error) {
    float P = error;             // Proportional term
    integral += error;           // Integral term accumulation
    float D = error - lastError; // Derivative term calculation

    // Calculate correction using PID formula
    float correction = Kp * P + Ki * integral + Kd * D;
    lastError = error;           // Update last error for next iteration

    return correction;
}

// ===================
// Adaptive PID Tuning: Adjusts Kp and Kd based on error magnitude for different turn intensities
// ===================
void adaptivePIDTuning(int error) {
    if (abs(error) > 300) {
        Kp = 10;
        Kd = 35;  // For sharp turns, use higher damping
    } else if (abs(error) > 150) {
        Kp = 8;
        Kd = 25;  // Moderate turn settings
    } else {
        Kp = 6;
        Kd = 15;  // For straights, softer response is sufficient
    }
}

// ===================
// Motor Speed Adjustment: Smoothly adjusts motor speeds based on the PID correction and error
// ===================
void adjustMotorSpeedsSmooth(float correction, int error) {
    // Map the absolute error to a dynamic base speed for smooth transitions
    int dynamicBaseSpeed = map(abs(error), 0, 350, baseSpeed, minSpeed + 30);
    dynamicBaseSpeed = constrain(dynamicBaseSpeed, minSpeed, baseSpeed);

    // Adjust left and right motor speeds based on the PID correction
    int leftSpeed = dynamicBaseSpeed + correction;
    int rightSpeed = dynamicBaseSpeed - correction;

    // Ensure speeds are within the defined bounds
    leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
    rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);

    // Set motor speeds for smooth turning
    moveMotorsSmooth(leftSpeed, rightSpeed);
}

// ===================
// Motor Control: Sends commands to both motors for smooth movement
// ===================
void moveMotorsSmooth(int leftSpeed, int rightSpeed) {
    setMotor(leftMotorIn1, leftMotorIn2, leftMotorEN, leftSpeed);
    setMotor(rightMotorIn1, rightMotorIn2, rightMotorEN, rightSpeed);
}

// ===================
// Set Individual Motor: Configures direction and speed for a given motor
// ===================
void setMotor(int in1, int in2, int en, int speed) {
    if (speed >= 0) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
    } else {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        speed = -speed; // Convert negative speed to positive for PWM
    }
    analogWrite(en, speed);
}

// ===================
// Stop Motors: Safely stops both motors
// ===================
void stopMotors() {
    analogWrite(leftMotorEN, 0);
    analogWrite(rightMotorEN, 0);
    digitalWrite(leftMotorIn1, LOW);
    digitalWrite(leftMotorIn2, LOW);
    digitalWrite(rightMotorIn1, LOW);
    digitalWrite(rightMotorIn2, LOW);
}

// ===================
// Lost Line Recovery: Executes a U-turn spin search when the line is lost
// ===================
void handleLostLine() {
    // Initialize search parameters when line is lost for the first time
    if (!searching) {
        searching = true;
        lostTime = millis();
        stopMotors();
        delay(50);
    }

    unsigned long searchDuration = millis() - lostTime;

    // Spin search behavior based on the duration of the lost signal
    if (searchDuration < 400) {
        if (lastError < 0) {
            moveMotorsSmooth(-80, 80);  // Spin left
        } else {
            moveMotorsSmooth(80, -80);  // Spin right
        }
    } else {
        if (lastError < 0) {
            moveMotorsSmooth(-120, 120);
        } else {
            moveMotorsSmooth(120, -120);
        }
    }
}

// ===================
// T-Junction Detection: Determines if the robot is at a T-Junction based on sensor activation
// ===================
bool detectTJunction(int sensorValues[]) {
    int activeSensors = 0;
    // Count the number of sensors that detect the line
    for (int i = 0; i < 8; i++) {
        if (sensorValues[i] == 1) activeSensors++;
    }
    // T-Junction is detected if nearly all sensors are active
    return activeSensors >= 7;
}
