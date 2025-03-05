# Line Follower Robot with PID Control – Updated Hardware Version

Welcome to the updated Line Follower Robot project! This iteration features a completely revamped hardware setup along with improved PID control code to ensure smooth and efficient track following.

## Table of Contents
- [Overview](#overview)
- [Hardware Components](#hardware-components)
- [Circuit Diagram & Wiring](#circuit-diagram--wiring)
- [Software & PID Control Code](#software--pid-control-code)
- [Setup & Assembly](#setup--assembly)
- [Troubleshooting](#troubleshooting)
- [Future Improvements](#future-improvements)
- [Resources](#resources)

## Overview
In this updated version, we replaced the previous chassis and motor setup with new components to boost performance:
- **New Chassis:** Provides a sturdier, more stable platform.
- **N20 Motors:** Upgraded motors for improved power and response.
- **Buck Converter:** Delivers a stable 5V output from battery voltage.
- **Compatible Wheels:** New wheels designed to work seamlessly with the updated chassis and motors.
- **3 Battery Holder with 3 Batteries:** Enhanced power management for longer operation.
- **Retained Motor Controller & Arduino:** Continues to use the existing motor controller and Arduino platform, integrated with updated PID control code.

## Hardware Components
The updated robot comprises the following parts:
- **Chassis:** New design for enhanced stability.
- **Motors:** N20 motors (for better torque and speed control).
- **Buck Converter:** Regulates battery voltage to a steady 5V.
- **Wheels:** New, compatible wheels.
- **Battery Holder & Batteries:** A 3-battery holder with three batteries for improved runtime.
- **Arduino Board:** (e.g., Arduino UNO) for processing and control.
- **IR Sensor Array:** 8-channel IR sensors for accurate line detection.
- **Motor Controller:** For controlling motor speed and direction.
- **Miscellaneous:** Wires, resistors, and other essential components.

## Circuit Diagram & Wiring
Below is a high-level wiring overview:
- **Motors & Motor Controller:**  
  - Connect the N20 motors to the motor controller.
  - Wire the controller outputs to the Arduino as defined in the code.
- **Buck Converter:**  
  - Connect the battery holder to the buck converter.
  - Use the converter’s regulated 5V output to power the Arduino and sensor array.
- **IR Sensor Array:**  
  - Connect each sensor to the designated digital pins.
- **Power Supply:**  
  - Use the 3-battery holder via the buck converter for a stable power source.

Refer to the detailed circuit diagrams provided in the repository for exact pin connections.

## Software & PID Control Code
The PID control code has been refined to minimize oscillations and ensure smooth tracking. Notable updates include:
- **Reduced Derivative Gain (Kd):** Lowered from 300 to 150 to reduce overreaction to noise.
- **Deadband Implementation:** Small errors (less than ±10) are ignored to prevent unnecessary corrections.

Below is the updated PID code:

```cpp
// Line Follower Robot with PID Control (Modified Code)

// Pin Definitions
const int leftMotorIn1 = 4;
const int leftMotorIn2 = 5;
const int leftMotorEN = 6;

const int rightMotorIn1 = 7;
const int rightMotorIn2 = 8;
const int rightMotorEN = 9;

// 8-Channel IR Sensor Pins
const int sensorPins[8] = {2, 3, 10, 11, 12, A0, A1, A2};

// Sensor Weights - Center is 0, left is negative, right is positive
const int weights[8] = {-350, -250, -150, -50, 50, 150, 250, 350};

// PID Constants (Tunable)
// Reduced Kd from 300 to 150 to lower sensitivity to noise
float Kp = 50;     // Proportional Gain
float Ki = 0;      // Integral Gain (often small for line followers)
float Kd = 150;    // Derivative Gain

// PID Variables
int lastError = 0;
float integral = 0;

// Base Speed (Tunable - for straight paths)
int baseSpeed = 70;      // Adjust based on motor power & track conditions
int minSpeed = 40;       // Minimum speed during sharp turns

void setup() {
    Serial.begin(9600);

    // Motor Pins
    pinMode(leftMotorIn1, OUTPUT);
    pinMode(leftMotorIn2, OUTPUT);
    pinMode(leftMotorEN, OUTPUT);

    pinMode(rightMotorIn1, OUTPUT);
    pinMode(rightMotorIn2, OUTPUT);
    pinMode(rightMotorEN, OUTPUT);

    // Sensor Pins
    for (int i = 0; i < 8; i++) {
        pinMode(sensorPins[i], INPUT);
    }

    stopMotors();
    delay(1000);  // Stabilize sensors
}

void loop() {
    int sensorValues[8];
    readSensors(sensorValues);

    int error = computeError(sensorValues);

    float correction = computePID(error);

    adjustMotorSpeeds(correction, error);

    delay(6);  // Short delay for stability
}

void readSensors(int sensorValues[]) {
    for (int i = 0; i < 8; i++) {
        int reading = digitalRead(sensorPins[i]);
        sensorValues[i] = (reading == LOW) ? 1 : 0;  // Black line detected = 1
    }
}

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
        return 0;  // No line detected - stop or handle recovery
    }

    int error = weightedSum / activeSensors;

    // Deadband to ignore minor errors caused by noise
    if (abs(error) < 10) {
        error = 0;
    }
    
    return error;
}

float computePID(int error) {
    float P = error;
    integral += error;
    float D = error - lastError;

    float output = Kp * P + Ki * integral + Kd * D;

    lastError = error;

    return output;
}

void adjustMotorSpeeds(float correction, int error) {
    int dynamicBaseSpeed = map(abs(error), 0, 350, baseSpeed, minSpeed);
    dynamicBaseSpeed = constrain(dynamicBaseSpeed, minSpeed, baseSpeed);

    int leftSpeed = dynamicBaseSpeed + correction;
    int rightSpeed = dynamicBaseSpeed - correction;

    leftSpeed = constrain(leftSpeed, -255, 255);
    rightSpeed = constrain(rightSpeed, -255, 255);

    moveMotors(leftSpeed, rightSpeed);
}

void moveMotors(int leftSpeed, int rightSpeed) {
    // Handle left motor (reversed wiring corrected here)
    if (leftSpeed >= 0) {
        digitalWrite(leftMotorIn1, HIGH);
        digitalWrite(leftMotorIn2, LOW);
    } else {
        digitalWrite(leftMotorIn1, LOW);
        digitalWrite(leftMotorIn2, HIGH);
        leftSpeed = -leftSpeed;  // Use absolute speed for PWM
    }
    analogWrite(leftMotorEN, leftSpeed);

    // Handle right motor
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

void stopMotors() {
    analogWrite(leftMotorEN, 0);
    analogWrite(rightMotorEN, 0);

    digitalWrite(leftMotorIn1, LOW);
    digitalWrite(leftMotorIn2, LOW);
    digitalWrite(rightMotorIn1, LOW);
    digitalWrite(rightMotorIn2, LOW);
}
