# Line Follower Robot with PID Control

## Project Overview

### Purpose
An autonomous robotic platform designed to navigate a predefined track using advanced line-following techniques and PID control methodology.

## Hardware Configuration

### Core Components
- **Microcontroller:** Arduino UNO R3
- **Motor Control:** L298N Motor Driver Module
- **Locomotion:** N20 DC Gear Motors (3V-12V)
- **Sensing:** 8-Channel IR Sensor Array
- **Power Management:** LM2596 Buck Converter
- **Power Source:** 3-Battery Holder (12V)

### Key Hardware Improvements
- Upgraded chassis for enhanced stability
- N20 motors for improved power and response
- LM2596 buck converter for stable voltage regulation
- 8-channel IR sensor array for precise line detection

## Circuit Connections

### Motor Driver Connections
| L298N Pin | Arduino Pin | Function |
|-----------|-------------|----------|
| IN1 | 4 | Left Motor Direction 1 |
| IN2 | 5 | Left Motor Direction 2 |
| ENA | 6 | Left Motor Speed PWM |
| IN3 | 7 | Right Motor Direction 1 |
| IN4 | 8 | Right Motor Direction 2 |
| ENB | 9 | Right Motor Speed PWM |

### IR Sensor Array Connections
| Sensor Index | Arduino Pin |
|--------------|-------------|
| Sensor 1 | 2 |
| Sensor 2 | 3 |
| Sensor 3 | 10 |
| Sensor 4 | 11 |
| Sensor 5 | 12 |
| Sensor 6 | A0 |
| Sensor 7 | A1 |
| Sensor 8 | A2 |

## PID Control Strategy

### Control Parameters
- **Proportional Gain (Kp):** 12
- **Integral Gain (Ki):** 0
- **Derivative Gain (Kd):** 36

### Key Control Features
- Weighted error calculation
- Deadband implementation
- Minimal oscillation design
- Dynamic motor speed adjustment

## Software Architecture

### Core Modules
1. Sensor Reading Module
2. Error Computation Module
3. PID Control Module
4. Motor Control Module

### Key Functions
- `readSensors()`: Capture IR sensor readings
- `computeError()`: Calculate line deviation
- `computePID()`: Generate correction values
- `adjustMotorSpeeds()`: Modify motor output

## Development Methodology

### Testing Phases
1. Individual Component Validation
2. Integrated System Testing
3. Track Performance Evaluation
4. PID Parameter Refinement

## Troubleshooting Guidelines

### Common Issues
- Incorrect motor rotation
- Sensor misalignment
- Unstable line tracking
- Power supply inconsistencies

## Future Enhancement Roadmap
- Adaptive PID tuning algorithm
- Wireless debugging interface
- Advanced sensor fusion

---
