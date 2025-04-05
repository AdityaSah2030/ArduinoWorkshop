/*
 * Line Following Robot with PID Control
 * 
 * Hardware:
 * - Arduino Nano
 * - TB6612FNG Motor Driver
 * - HC-05 Bluetooth Module (for PID tuning only)
 * - 8-Channel IR Sensor Array (RLSO8)
 * - LM2596 Buck Converter
 * 
 * Bluetooth is used only for tuning Kp, Ki, and Kd values
 */

// ===================
// Motor Driver Pins Configuration
// ===================
// Define pins connected to the motor driver for both left and right motors.
#define PWMA 5     // Left Motor PWM (speed control)
#define AIN1 3     // Left Motor Direction Pin 1
#define AIN2 4     // Left Motor Direction Pin 2
#define STBY 6     // Standby Pin (enables/disables the motor driver)
#define PWMB 9     // Right Motor PWM (speed control)
#define BIN1 10    // Right Motor Direction Pin 1
#define BIN2 12    // Right Motor Direction Pin 2

// ===================
// Sensor Array Pins Configuration
// ===================
// Define analog pins connected to the 8-channel IR sensor array.
// SENSOR1 is the rightmost sensor and SENSOR8 is the leftmost sensor.
#define SENSOR1 A0  
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4
#define SENSOR6 A5
#define SENSOR7 A6
#define SENSOR8 A7  

// ===================
// LED Pin Configuration
// ===================
// LED to indicate special conditions like checkpoints.
#define LED_PIN 13

// ===================
// PID and Sensor Constants
// ===================
// SENSOR_COUNT: Total number of sensors used.
// BLACK_LINE: Determines whether the robot follows a black line (1) or a white line (0).
// SENSOR_THRESHOLD: Analog threshold value to convert sensor readings into binary.
#define SENSOR_COUNT 8
#define BLACK_LINE 1      
#define SENSOR_THRESHOLD 500  

// ===================
// PID Controller Parameters (Initial Values)
// ===================
// These values can be tuned via Bluetooth commands.
int Kp = 25;    // Proportional gain
int Ki = 0;     // Integral gain
int Kd = 75;    // Derivative gain

// ===================
// Robot Movement Parameters
// ===================
// Define base speeds and other constants used for controlling the robot's movement.
int baseSpeed = 90;      // Base motor speed (0-255)
int maxSpeed = 130;       // Maximum motor speed limit
int brakeTime = 50;       // Time in milliseconds for braking maneuvers
int turnSpeedReduction = 30; // Percentage reduction in speed when turning

// ===================
// PID Calculation Variables
// ===================
// Variables used in the PID calculations.
int lastError = 0;       // Previous error for derivative calculation
int integral = 0;        // Accumulated error for integral calculation

// ===================
// Control and Status Variables
// ===================
// isRunning: Indicates whether the robot is actively following the line.
// bluetoothData: Stores incoming data from the Bluetooth module for PID tuning.
// errorDirection: Tracks the last known direction of error when the line is lost.
bool isRunning = true;    
String bluetoothData = "";
int errorDirection = 0;   

// ===================
// Feedback Timing Variables
// ===================
// lastFeedbackTime: Last time feedback was sent via Bluetooth.
// FEEDBACK_INTERVAL: How often (in milliseconds) to send updated PID values.
unsigned long lastFeedbackTime = 0;
const unsigned long FEEDBACK_INTERVAL = 1000; // 1 second

// ===================
// Setup Function: Initializes Hardware and Communication
// ===================
void setup() {
  // Initialize Serial Communication for Bluetooth (and debugging)
  Serial.begin(9600);
  
  // ===================
  // Initialize Motor Driver Pins
  // ===================
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  // ===================
  // Initialize LED Pin
  // ===================
  pinMode(LED_PIN, OUTPUT);
  
  // ===================
  // Initialize Sensor Pins
  // ===================
  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  pinMode(SENSOR5, INPUT);
  pinMode(SENSOR6, INPUT);
  pinMode(SENSOR7, INPUT);
  pinMode(SENSOR8, INPUT);
  
  // Enable the motor driver by taking it out of standby mode
  digitalWrite(STBY, HIGH);
  
  // ===================
  // Print Startup Message and PID Tuning Instructions
  // ===================
  Serial.println("Line Follower Robot - Active");
  Serial.println("PID Tuning Commands:");
  Serial.println("p[value] - Set Kp value");
  Serial.println("i[value] - Set Ki value");
  Serial.println("d[value] - Set Kd value");
  Serial.println("Current PID values:");
  sendPIDValues();
}

// ===================
// Main Loop: Sensor Reading, PID Calculation, and Motor Control
// ===================
void loop() {
  // Process any incoming Bluetooth commands for PID tuning
  processBluetooth();
  
  // ===================
  // Sensor Reading
  // ===================
  // Read sensor array and get an 8-bit value representing the line position.
  uint8_t sensorValues = readSensors();
  
  // ===================
  // PID Calculation
  // ===================
  // Calculate the error value based on sensor readings.
  int error = calculateError(sensorValues);
  // Use the error to calculate the PID output.
  int pidValue = calculatePID(error);
  
  // ===================
  // Special Conditions Handling
  // ===================
  // Check if the robot is at a checkpoint (middle sensors detect the line)
  if (isCheckpoint(sensorValues)) {
    digitalWrite(LED_PIN, HIGH);  // Turn on LED at checkpoints
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  // Check if the robot is at a crossroad (combination of center and outer sensors)
  if (isCrossroad(sensorValues)) {
    // At crossroads, go straight for a short period to cross the intersection.
    moveForward(baseSpeed, baseSpeed);
    delay(100);  
  }
  // Check if the robot has reached the finish line (all sensors detect the line)
  else if (isFinishLine(sensorValues)) {
    // Apply a short brake, stop motors, and put motor driver in standby.
    shortBrake(150);
    stopMotors();
    digitalWrite(STBY, LOW);
    isRunning = false;
    Serial.println("Finish line detected - Stopped");
    delay(3000);  // Wait for 3 seconds at the finish line
    
    // Restart the robot after finishing.
    digitalWrite(STBY, HIGH);
    isRunning = true;
  }
  // Check if the robot has lost the line
  else if (isOutOfLine(sensorValues)) {
    // Handle the case where the robot is off the line.
    handleOutOfLine();
  }
  // Normal operation: Follow the line using PID control.
  else {
    // Calculate individual motor speeds based on the PID output.
    int leftSpeed = baseSpeed - pidValue;
    int rightSpeed = baseSpeed + pidValue;
    
    // Constrain motor speeds to within the allowed range.
    leftSpeed = constrain(leftSpeed, -maxSpeed, maxSpeed);
    rightSpeed = constrain(rightSpeed, -maxSpeed, maxSpeed);
    
    // Set the motor speeds accordingly.
    setMotorSpeeds(leftSpeed, rightSpeed);
  }
  
  // ===================
  // Periodic Feedback
  // ===================
  // Send current PID values over Bluetooth at set intervals.
  sendPeriodicFeedback();
}

// ===================
// Feedback Functions
// ===================

// Send current PID values over Bluetooth at regular intervals.
void sendPeriodicFeedback() {
  unsigned long currentTime = millis();
  if (currentTime - lastFeedbackTime >= FEEDBACK_INTERVAL) {
    lastFeedbackTime = currentTime;
    sendPIDValues();
  }
}

// Send current PID values to the Serial monitor (Bluetooth).
void sendPIDValues() {
  Serial.print("PID Values: Kp=");
  Serial.print(Kp);
  Serial.print(", Ki=");
  Serial.print(Ki);
  Serial.print(", Kd=");
  Serial.println(Kd);
}

// ===================
// Sensor Reading and Error Calculation
// ===================

// Reads the 8-channel IR sensor array and returns an 8-bit value representing sensor states.
uint8_t readSensors() {
  uint8_t sensorValue = 0;
  
  // Read each sensor using analogRead and compare against the threshold.
  // The reading is inverted based on whether we're following a black line.
  bool s1 = (analogRead(SENSOR1) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  bool s2 = (analogRead(SENSOR2) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  bool s3 = (analogRead(SENSOR3) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  bool s4 = (analogRead(SENSOR4) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  bool s5 = (analogRead(SENSOR5) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  bool s6 = (analogRead(SENSOR6) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  bool s7 = (analogRead(SENSOR7) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  bool s8 = (analogRead(SENSOR8) > SENSOR_THRESHOLD) ^ !BLACK_LINE;
  
  // Combine each sensor's binary result into a single byte.
  sensorValue = (s1 << 0) | (s2 << 1) | (s3 << 2) | (s4 << 3) | 
                (s5 << 4) | (s6 << 5) | (s7 << 6) | (s8 << 7);
                
  return sensorValue;
}

// Calculate the error value representing the deviation from the center of the line.
// Returns a value between -3500 and 3500.
int calculateError(uint8_t sensorValues) {
  float weightedSum = 0;   // Sum of weighted sensor positions
  int activeCount = 0;     // Count of sensors detecting the line
  
  // Loop through each sensor (0 to SENSOR_COUNT-1)
  for (int i = 0; i < SENSOR_COUNT; i++) {
    // If the sensor bit is set, add its weighted contribution.
    if (bitRead(sensorValues, i)) {
      weightedSum += i * 1000;
      activeCount++;
    }
  }
  
  int position = 0;
  if (activeCount > 0) {
    // Calculate the average position of the line.
    position = weightedSum / activeCount;
  }
  
  // Error is calculated as deviation from the center position (3500).
  int error = position - 3500;
  
  // Update error direction (used when line is lost)
  if (activeCount > 0) {
    if (error > 0) errorDirection = 1;
    else if (error < 0) errorDirection = -1;
  }
  
  // If no sensor is active, assign error based on the last known error direction.
  if (activeCount == 0) {
    if (errorDirection > 0) error = 3500;
    else if (errorDirection < 0) error = -3500;
  }
  
  return error;
}

// Calculate the PID output value based on the current error.
int calculatePID(int error) {
  // Proportional term is the error itself.
  int P = error;
  
  // Accumulate the error for the integral term.
  integral += error;
  // Constrain the integral term to prevent windup.
  integral = constrain(integral, -10000, 10000);
  int I = integral;
  
  // Derivative term is the difference between current error and last error.
  int D = error - lastError;
  lastError = error;
  
  // Combine the three PID components.
  int pidValue = (Kp * P) / 100 + (Ki * I) / 100 + (Kd * D) / 100;
  
  return pidValue;
}

// ===================
// Special Conditions: Checkpoints, Crossroads, and Finish Line Detection
// ===================

// Check if the robot is at a checkpoint (middle four sensors detect the line).
bool isCheckpoint(uint8_t sensorValues) {
  return (sensorValues & 0b00111100) == 0b00111100;
}

// Check if the robot is at a crossroad (center sensors and both outer sensors detect the line).
bool isCrossroad(uint8_t sensorValues) {
  return ((sensorValues & 0b00011000) == 0b00011000) && 
         ((sensorValues & 0b10000001) == 0b10000001);
}

// Check if the robot is at the finish line (all sensors detect the line).
bool isFinishLine(uint8_t sensorValues) {
  // Confirm finish line by checking if all bits are set.
  if (sensorValues == 0b11111111) {
    delay(100);  // Delay to avoid false detection
    uint8_t confirmValues = readSensors();
    return (confirmValues == 0b11111111);
  }
  return false;
}

// Check if the robot is completely off the line.
bool isOutOfLine(uint8_t sensorValues) {
  return sensorValues == 0;
}

// Handle the scenario when the robot loses the line.
void handleOutOfLine() {
  // Apply a short brake to prevent overshooting.
  shortBrake(brakeTime / 2);
  
  // Turn in the direction of the last known error.
  if (errorDirection < 0) {
    // Last known error indicates line was to the left, so turn left.
    turnLeft(baseSpeed * (100 - turnSpeedReduction) / 100);
  } else {
    // Otherwise, turn right.
    turnRight(baseSpeed * (100 - turnSpeedReduction) / 100);
  }
  
  // Continue turning until the line is detected again.
  unsigned long startTime = millis();
  while (isOutOfLine(readSensors())) {
    // Timeout after 2 seconds to avoid an infinite loop.
    if (millis() - startTime > 2000) {
      break;
    }
    delay(10);  // Small delay for stability
  }
}

// ===================
// Bluetooth Command Processing for PID Tuning
// ===================

// Continuously checks for Bluetooth commands and accumulates them.
void processBluetooth() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    // Check for end-of-line characters to determine command end.
    if (c == '\n' || c == '\r') {
      if (bluetoothData.length() > 0) {
        processCommand(bluetoothData);
        bluetoothData = "";
      }
    } else {
      bluetoothData += c;
    }
  }
}

// Process a complete Bluetooth command string to update PID parameters.
void processCommand(String command) {
  if (command.length() > 0) {
    // The command's first character determines which PID value to update.
    char cmd = command.charAt(0);
    
    switch (cmd) {
      case 'p':  // Update Kp value.
        Kp = command.substring(1).toInt();
        Serial.print("Kp set to: ");
        Serial.println(Kp);
        break;
        
      case 'i':  // Update Ki value.
        Ki = command.substring(1).toInt();
        // Reset the integral to avoid sudden jumps.
        integral = 0;  
        Serial.print("Ki set to: ");
        Serial.println(Ki);
        break;
        
      case 'd':  // Update Kd value.
        Kd = command.substring(1).toInt();
        Serial.print("Kd set to: ");
        Serial.println(Kd);
        break;
    }
    
    // Send the updated PID values after any change.
    sendPIDValues();
  }
}

// ===================
// Motor Control Functions
// ===================

// Set the speeds of the left and right motors.
void setMotorSpeeds(int leftSpeed, int rightSpeed) {
  // Set left motor direction based on the sign of leftSpeed.
  if (leftSpeed >= 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  } else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    leftSpeed = -leftSpeed;  // Convert negative speed to positive for PWM.
  }
  
  // Set right motor direction based on the sign of rightSpeed.
  if (rightSpeed >= 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  } else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    rightSpeed = -rightSpeed;  // Convert negative speed to positive.
  }
  
  // Set motor speeds using PWM.
  analogWrite(PWMA, leftSpeed);
  analogWrite(PWMB, rightSpeed);
}

// Move forward by setting both motors to drive forward.
void moveForward(int leftSpeed, int rightSpeed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  
  analogWrite(PWMA, leftSpeed);
  analogWrite(PWMB, rightSpeed);
}

// Turn left by driving the left motor in reverse and right motor forward.
void turnLeft(int speed) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

// Turn right by driving the right motor in reverse and left motor forward.
void turnRight(int speed) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  
  analogWrite(PWMA, speed);
  analogWrite(PWMB, speed);
}

// Stop both motors completely.
void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}

// Short brake: Quickly stops the motors by electrically shorting motor terminals.
void shortBrake(int duration) {
  // Set PWM to zero.
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  
  // Short the motor terminals to quickly halt motion.
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);
  
  delay(duration);  // Hold the brake for the specified duration.
}
