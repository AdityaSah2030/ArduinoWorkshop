# Arduino Line Following Robot Project

This repository contains code for an Arduino-based line following robot with obstacle detection capabilities. This guide will help you set up both the hardware and software components needed for this project.

## Table of Contents
- [Required Components](#required-components)
- [Software Setup](#software-setup)
- [Hardware Setup](#hardware-setup)
- [Project Files Description](#project-files-description)
- [Online Simulation](#online-simulation)
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

## Software Setup

### Installing Arduino IDE
1. Visit the official Arduino website: [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Download the Arduino IDE for your operating system (Windows, Mac, or Linux)
3. Install the software by following the installation wizard
4. Launch Arduino IDE after installation

### First-Time Setup
1. Connect your Arduino UNO to your computer using a USB cable
2. In Arduino IDE, go to Tools → Board and select "Arduino UNO"
3. Go to Tools → Port and select the port your Arduino is connected to
   - Windows: Usually shows as COM3, COM4, etc.
   - Mac/Linux: Usually shows as /dev/ttyUSB0 or similar

## Hardware Setup

### LED Connections
- Connect LEDs to digital pins (specified in LED_Indicator.ino)
- Use 220Ω resistors in series with each LED
- Long leg (anode) connects to Arduino pin through resistor
- Short leg (cathode) connects to GND

### IR Sensors
- VCC to 5V
- GND to GND
- OUT to digital pins (specified in IR_Sensor.ino)

### Motor Driver (L298N)
- ENA & ENB to Arduino PWM pins
- IN1, IN2, IN3, IN4 to Arduino digital pins
- 12V to external power supply
- GND to both Arduino GND and power supply GND

### Ultrasonic Sensor
- VCC to 5V
- GND to GND
- TRIG to digital pin
- ECHO to digital pin

## Project Files Description

1. `IR_Sensor.ino`
   - Basic IR sensor testing
   - Learn how to read IR sensor values

2. `IR_Sensor_with_LED.ino`
   - Combines IR sensors with LED indicators
   - Shows sensor detection through LED feedback

3. `LED_Blinking.ino`
   - Basic LED control tutorial
   - Learn digital pin control

4. `LED_Indicator.ino`
   - Advanced LED control patterns
   - Status indication implementation

5. `Ultrasonic_Distance_Sensor.ino`
   - Distance measurement implementation
   - Obstacle detection logic

## Online Simulation

You can test your code before implementing it on hardware using Wokwi:

1. Visit [https://wokwi.com/](https://wokwi.com/)
2. Click "Create New Project"
3. Select "Arduino UNO"
4. Copy and paste the desired code
5. Add components using the "+" button
6. Connect components according to the diagram
7. Click "Start Simulation" to test

### Wokwi Tips
- Use the built-in serial monitor to debug
- Save your projects by creating an account
- Share simulations with others using the "Share" button
- Check the "Examples" section for learning purposes

## Troubleshooting

### Common Issues and Solutions

1. LEDs not lighting up
   - Check polarity (long leg should connect to Arduino pin through resistor)
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

### Getting Help
- Check Arduino's official forums
- Join Arduino communities on Reddit (/r/arduino)
- Document your issues with clear descriptions and photos
- Share your code when asking for help

## Contributing
Feel free to fork this project and submit improvements through pull requests. We welcome contributions that make this guide more helpful for beginners.
