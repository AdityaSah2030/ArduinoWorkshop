# Arduino Line Following Robot Project

## Table of Contents
- [Required Components](#required-components)
- [Circuit Diagrams](#circuit-diagrams)
- [Project Evolution](#project-evolution)
- [Simulation and Testing](#simulation-and-testing)
- [Troubleshooting](#troubleshooting)

## Required Components

### Hardware
- 1x Arduino UNO board
- 1x Breadboard
- 2x IR Sensors
- 2x LEDs
- 2x DC Motors
- 1x L298N Motor Driver
- 1x Ultrasonic Distance Sensor (HC-SR04)
- Multiple Resistors (220Ω for LEDs)
- Jumper Wires (Male-to-Male, Male-to-Female, Female-to-Female)
- Power Supply (7-12V) for motors

### Component Specifications

#### IR Sensors
- Operating Voltage: 3.3V-5V
- Digital output
- Infrared reflection detection
- Typically used in pairs for line tracking

#### DC Motors
- Voltage: 6V-12V
- Speed: Varies based on specific model
- Recommended to use with motor driver

#### Ultrasonic Sensor (HC-SR04)
- Operating Voltage: 5V
- Detection Range: 2cm-400cm
- Accuracy: ±3mm

#### L298N Motor Driver
- Supports 2 DC motors
- Input Voltage: 5V-35V
- Max Current: 2A per channel
- Includes speed control via PWM

## Circuit Diagrams

### LED Connection
```
Arduino Pin --> Resistor (220Ω) --> LED(+) --> GND
```

### IR Sensor Connection
```
VCC --> 5V
GND --> GND
OUT --> Digital Pin
```

### Ultrasonic Sensor Connection
```
VCC --> 5V
GND --> GND
TRIG --> Digital Pin
ECHO --> Digital Pin
```

### Motor Driver (L298N) Connection
```
ENA/ENB --> PWM Pins
IN1-IN4 --> Digital Pins
12V --> External Power
GND --> Arduino GND + Power GND
```

## Project Evolution

### Learning Stages

1. **LED Blinking (`LED_Blinking.ino`)**
   - First introduction to digital pin control
   - Understanding basic output mechanisms
   - Learning to control LED state programmatically

2. **IR Sensor Basics (`IR_Sensor.ino`)**
   - Introduction to digital input
   - Understanding sensor reading techniques
   - Exploring digital signal interpretation

3. **IR Sensor with LED Feedback (`IR_Sensor_with_LED.ino`)**
   - Combining input and output
   - Using LEDs to visualize sensor states
   - Basic interaction between sensors and actuators

4. **Advanced LED Control (`LED_Indicator.ino`)**
   - Creating complex LED patterns
   - Implementing state-based LED behaviors
   - Exploring timing and sequencing

5. **Distance Measurement (`Ultrasonic_Distance_Sensor.ino`)**
   - Introducing ultrasonic sensor technology
   - Learning distance calculation methods
   - Understanding sensor timing and signal processing
  
6. **Line Follower Robot (`Basic_LFR.ino`)**
   - First integration of IR sensors and motor driver
   - Basic line detection and navigation logic
   - Uses PWM signals for speed control
   - Foundation for more advanced line follower projects

### Final Project: Line Following Robot
The culmination of our learning, integrating all previous knowledge into a functional robotic system.

## Simulation and Testing

### Online Simulation with Wokwi
1. Visit [https://wokwi.com/](https://wokwi.com/)
2. Create New Arduino UNO Project
3. Add components using the "+" button
4. Connect components according to project diagrams
5. Use serial monitor for debugging
6. Share and collaborate on simulations

## Troubleshooting

### Common Issues

1. LEDs not lighting up
   - Check polarity (long leg to Arduino pin)
   - Verify resistor values
   - Test different digital pins

2. Motors not moving
   - Check power supply connections
   - Verify L298N jumper settings
   - Test motor connections individually
   - Check code for correct pin assignments

3. IR Sensors not detecting
   - Adjust sensor height from ground
   - Check for loose connections
   - Verify voltage levels
   - Calibrate sensors if needed

4. Ultrasonic sensor readings incorrect
   - Check wiring connections
   - Verify TRIG and ECHO pin assignments
   - Ensure stable 5V power supply
   - Test in different environments

## Recommended Next Steps
1. Implement PID control for smoother navigation
2. Add Bluetooth/wireless control capabilities
3. Integrate advanced sensor fusion techniques

## Resources
- [Arduino Project Hub](https://create.arduino.cc/projecthub)
- [Robotics Online Tutorials](https://www.robotshop.com/community/)
- [Embedded Systems Learning Platforms](https://www.embedded.com/)

---
