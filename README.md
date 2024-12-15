# Robot Control and Navigation System

## Overview

This project implements a robotics system capable of real-time control, autonomous navigation, and environment interaction. It features a web-based interface, multiple sensors, and PID-based motor control for precise movements. The modular design enables easy integration and extension for additional functionalities.

---

## Features

### 1. Robot Control via Web Interface
- **HTML and JavaScript-based Control Panel**:
  - Directional controls (Forward, Backward, Left, Right).
  - Adjustable speed slider.
  - Real-time state monitoring (Health, RPM, Speed, Bearing, Position).
- **Canvas-based Visualization**:
  - Interactive map showing current position and targets.
  - Click-to-set navigation waypoints.

### 2. Autonomous Navigation and Attack Modes
- **Autonomous Modes**:
  - Navigation to predefined points (e.g., Red/Blue Nexus, Towers).
  - Wall-following for obstacle avoidance.
  - Attack closest enemy structure or static targets.
- **Decision Logic**:
  - Uses Euclidean distance and bearing calculations.
  - Target and obstacle tracking with tolerance-based precision.

### 3. PID Motor Control
- Implements **Proportional-Integral-Derivative (PID)** controllers for motor speed regulation:
  - Left and right motors controlled separately.
  - Parameters dynamically tuned for optimal performance:
    - Proportional Gain (`lkp`, `rkp`).
    - Integral Gain (`lki`, `rki`).
    - Derivative Gain (`lkd`, `rkd`).
- Ensures stable and accurate motor speed adjustments for navigation.

### 4. Sensor Integration
- **Sensors Used**:
  - **Sharp IR**: Measures obstacle distances.
  - **Time-of-Flight (TOF)**: Tracks forward and rightward distances.
  - **Vive Sensors**: Provides precise positional coordinates.
  - **Encoders**: Tracks motor revolutions for speed and position.
- **Sensor Data Processing**:
  - Median filtering for noise reduction.
  - Continuous updates for localization and speed feedback.

### 5. Modular Architecture
- **Robot Class**:
  - Manages movement states (e.g., forward, backward, attack).
  - Processes sensor inputs and updates motor commands.
- **Sensor Class**:
  - Updates sensor readings, including distance and bearing.
  - Provides health monitoring and localization feedback.
- **Point Class**:
  - Facilitates spatial calculations (e.g., distances, angles).

---

## Technical Details

### Motor Control and PID
- **PID Algorithm**:
  - Calculates error as the difference between desired and actual motor speeds.
  - Adjusts motor PWM signals to minimize error.
  - Includes anti-windup mechanisms to prevent integral oversaturation.

### State Machine
- **Robot States**:
  - `0`: Stop.
  - `1`: Move forward.
  - `2`: Move backward.
  - `3`: Turn left.
  - `4`: Turn right.
  - `5`: Navigate to target.
  - `6`: Attack closest target.
  - `7`: Attack static structure.
  - `8`: Navigate up a ramp.
- **Autonomous Substates**:
  - For navigation: Approach target, turn, wall-follow, and stop when close.
  - For attack: Scan, orient, and approach target.

### Autonomous Constants (`config.h`)
- **Configurable Parameters**:
  - `distTolerance`: Minimum distance to consider the target reached.
  - `bearingTolerance`: Allowed deviation from desired angle.
  - PID tuning constants for adaptive motor control.
- **Predefined Points/Orientations**:
  - Red Nexus, Blue Nexus, Towers.

---

## Files and Structure

| File            | Description                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `web.h`         | Defines the web server and control panel interface.                         |
| `auton.h`       | Functions for autonomous navigation and decision-making.                    |
| `config.h`      | Configuration file with constants, pin definitions, and feature toggles.    |
| `html510.h`     | Utilities for handling web interface communication.                         |
| `point.h`       | Point class for spatial calculations like distances and angles.             |
| `robot.h`       | Core robot class handling movement, states, and control logic.              |
| `sensors.h`     | Manages sensors (TOF, Vive, IR) and updates localization and health data.   |
| `SharpIR.h`     | Library for Sharp IR distance measurement.                                  |
| `vive510.h`     | Tracks position using Vive sensors.                                         |
| `final_project.ino` | Main Arduino sketch integrating all modules and logic.                  |

---

## Setup and Installation

### 1. Hardware Configuration
- Assemble the robot, connecting sensors and motors.
- Ensure all pins match the configuration in `config.h`.

### 2. Software Setup
- Install Arduino IDE with required libraries:
  - `Adafruit_VL53L1X`
  - `SharpIR`
- Compile and upload `final_project.ino` to the microcontroller.

### 3. Web Interface
- Connect to the robot's WiFi network.
- Open the provided web interface in a browser for control.

---

## Usage

### Manual Control
- Use the web interface buttons to control movement (forward, backward, left, right).
- Adjust speed using the slider.

### Autonomous Modes
- Select from predefined tasks such as attacking structures or wall-following.
- Monitor real-time feedback on position, speed, and state.

---

## Future Improvements
- Integrate additional sensors for better obstacle avoidance.
- Implement advanced path planning algorithms.
- Add a feature for mapping unknown environments.

---

## Acknowledgments
- Developed for MEAM510 at the University of Pennsylvania.
- `'html510'` based on example frameworks provided by the course.

---
