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
- **Motor Driver:** Using the **L298N Motor Driver** for controlling the N20 motors.
- **IR Sensor Array:** Upgraded to an **8-Channel IR Sensor Array** for enhanced line detection.
- **Arduino Board:** (e.g., Arduino UNO) for processing and control.

## Hardware Components
The updated robot comprises the following parts:
- **Chassis:** New design for enhanced stability.
- **Motors:** N20 motors (for better torque and speed control).
- **Buck Converter:** Regulates battery voltage to a steady 5V.
- **Wheels:** New, compatible wheels.
- **Battery Holder & Batteries:** A 3-battery holder with three batteries for improved runtime.
- **Arduino Board:** (e.g., Arduino UNO) for processing and control.
- **Motor Driver:** L298N Motor Driver for driving the motors.
- **IR Sensor Array:** Upgraded to an 8-channel IR sensor array for accurate line detection.
- **Miscellaneous:** Wires, resistors, and other essential components.

## Circuit Diagram & Wiring
Below is a high-level wiring overview:
- **Motors & Motor Driver (L298N):**  
  - Connect the N20 motors to the L298N motor driver.
  - Wire the driver outputs to the Arduino as defined in the code.
- **Buck Converter:**  
  - Connect the battery holder to the buck converter.
  - Use the converter’s regulated 5V output to power the Arduino and sensor array.
- **IR Sensor Array:**  
  - Connect the upgraded 8-channel IR sensor array to the designated Arduino digital pins.
- **Power Supply:**  
  - Use the 3-battery holder via the buck converter for a stable power source.

Refer to the detailed circuit diagrams provided in the repository for exact pin connections.

## Software & PID Control Code
The PID control code has been refined to minimize oscillations and ensure smooth tracking. Notable updates include:
- **Reduced Derivative Gain (Kd):** Lowered from 300 to 150 to reduce overreaction to noise.
- **Deadband Implementation:** Small errors (less than ±10) are ignored to prevent unnecessary corrections.

The complete PID control code is available in the **`line_follower_pid.ino`** file in this repository.

## Setup & Assembly
1. **Hardware Assembly:**
   - **Chassis & Motors:** Mount the N20 motors on the new chassis and attach the compatible wheels.
   - **Motor Driver:** Install the L298N motor driver and connect the N20 motors to it.
   - **Power Supply:** Secure the 3-battery holder, insert the batteries, and connect them through the buck converter to deliver a stable 5V.
   - **Wiring:** Connect the upgraded 8-channel IR sensor array to the designated Arduino pins and wire the motor driver between the motors and Arduino as per the code.
   
2. **Software Setup:**
   - Install the Arduino IDE and necessary drivers.
   - Open the **`line_follower_pid.ino`** file in the IDE.
   - Select the appropriate board and COM port.
   - Upload the code to your Arduino.

3. **Testing:**
   - Place your robot on the test track.
   - Observe its line-following behavior.
   - Fine-tune PID parameters if necessary for optimal performance.

## Troubleshooting
- **Motor Operation:**  
  Verify that the N20 motors and L298N motor driver are properly connected and functioning.
- **Power Supply:**  
  Ensure the buck converter outputs a stable 5V.
- **Sensor Calibration:**  
  Clean and align the upgraded 8-channel IR sensors to guarantee accurate line detection.
- **PID Tuning:**  
  Adjust the Kp and Kd values if the robot oscillates or if performance isn’t as expected.

## Future Improvements
- **Enhanced Sensor Fusion:**  
  Consider integrating additional sensors (e.g., gyroscope) for better navigation.
- **Remote Control:**  
  Add Bluetooth or Wi-Fi modules for wireless control and telemetry.
- **Advanced Control Algorithms:**  
  Explore adaptive PID tuning or machine learning techniques for further optimization.

## Resources
- [Arduino Official Documentation](https://www.arduino.cc/reference/en/)
- [PID Control Fundamentals](https://en.wikipedia.org/wiki/PID_controller)
- [Component Suppliers](https://www.adafruit.com/)

---

This repository documents the complete process of building and optimizing the line follower robot with updated hardware and refined PID control. Contributions, issues, and suggestions are highly welcome!