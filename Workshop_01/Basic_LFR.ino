// Define pin assignments for infrared sensors and motor control outputs
#define IR1 4      // IR sensor 1 connected to digital pin 4
#define IR2 8      // IR sensor 2 connected to digital pin 8

// Define motor driver control pins
#define in1 3      // Motor driver input 1 (controls one motor direction)
#define in2 5      // Motor driver input 2 (controls one motor direction)
#define in3 9      // Motor driver input 3 (controls other motor direction)
#define in4 10     // Motor driver input 4 (controls other motor direction)

void setup() {
  // Initialize IR sensor pins as inputs
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Initialize motor driver pins as outputs
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // Read the current state of both IR sensors
  int IR1value = digitalRead(IR1);
  int IR2value = digitalRead(IR2);
  
  // Determine robot action based on sensor readings:
  
  // When both sensors return 0 (no obstacle detected), move forward.
  if (IR1value == 0 && IR2value == 0) {
    moveForward();
  }
  // When IR sensor 1 detects an obstacle (1) and sensor 2 does not (0), turn left.
  else if (IR1value == 1 && IR2value == 0) {
    turnLeft();
  }
  // When IR sensor 2 detects an obstacle (1) and sensor 1 does not (0), turn right.
  else if (IR1value == 0 && IR2value == 1) {
    turnRight();
  }
  // When both sensors detect obstacles, stop the robot.
  else {
    Stop();
  }
}

// Function to move the robot forward
void moveForward() {
   // Set motor speeds for forward movement:
   // Motors on both sides move at high speed (PWM value 250)
   analogWrite(in1, 250);
   analogWrite(in2, 0);
   analogWrite(in3, 250);
   analogWrite(in4, 0);
}

// Function to turn the robot left
void turnLeft() {
   // Reduce speed on left motor while right motor is stopped to initiate a left turn
   analogWrite(in1, 150);
   analogWrite(in2, 0);
   analogWrite(in3, 0);
   analogWrite(in4, 0);
}

// Function to turn the robot right
void turnRight() {
   // Reduce speed on right motor while left motor is stopped to initiate a right turn
   analogWrite(in1, 0);
   analogWrite(in2, 0);
   analogWrite(in3, 150);
   analogWrite(in4, 0);
}

// Function to stop the robot
void Stop() {
   // Stop both motors by setting all outputs to 0
   analogWrite(in1, 0);
   analogWrite(in2, 0);
   analogWrite(in3, 0);
   analogWrite(in4, 0);
}
