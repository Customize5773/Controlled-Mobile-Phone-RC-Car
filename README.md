# NodeMCU Car Control: A Modular Two-Wheel Robot Controller 🤖🚗⚡

## Overview 📚

![NodeMCU-ESP8266-WiFi-Controlled-Robotic-Car](https://github.com/user-attachments/assets/59c782b4-405d-44bd-a470-132928bfe9ab)

>*Source:https://how2electronics.com/wifi-controlled-robot-using-esp8266-android-app/*

NodeMCU Car Control is a robust and flexible control system designed for two-wheeled robots. Leveraging the NodeMCU (ESP8266) and an L298N motor driver, this project offers remote robot control via an integrated HTTP server. Built with a modular, object-oriented approach, it targets real-world competitive robotics challenges by providing an affordable and scalable solution ideal for mechatronics enthusiasts and students.

---

> **Backstory 📖**
In our early days of diving into competitive robotics—whether it was the intensity of sumo battles or the spirited excitement of robot soccer—we quickly learned that the toughest opponent wasn’t always on the arena floor. It was the endless string of technical hiccups that kept us up at night. We faced component limitations that made even the simplest tasks feel like climbing Everest, performance issues that seemed to pop up when we least expected them, and the ever-present challenge of navigating expensive parts on a shoestring budget. And, not to mention, our school’s enthusiasm (or lack thereof) could best be described as tepid at best when it came to supporting our wild ideas.

> Despite these hurdles, our shared passion for innovation kept us going. Picture a group of sleep-deprived enthusiasts huddled in a cramped, makeshift workshop—where laughter was as common as the sound of soldering irons. We experimented with every idea that came to mind, learned from spectacular failures (yes, there were plenty of “oops” moments), and gradually figured out what worked. Tired of being boxed in by proprietary systems that limited our creativity, we turned to the versatile and cost-effective NodeMCU. It was like discovering a secret weapon that allowed us to dream big without breaking the bank.

> This project was born out of those long nights, countless trials, and the determination to overcome every obstacle in our path. It isn’t just a tool for better performance—it’s a tribute to our resilience, our ability to laugh at our missteps, and our relentless pursuit of solutions under real-world pressure.

---

## Hardware Components ⚙️🔋

The project requires the following electronic components:

| **Component**         | **Details**                                                             |
|-----------------------|-------------------------------------------------------------------------|
| ESP8266 Module        | NodeMCU or any ESP8266-based development board                           |
| Motor Driver Module   | L298N                                                                    |
| **DC Motors (2x)**    | Right and Left motors as dictated by the connections below                |
| Jumper Wires          | For connecting the ESP8266 to the motor driver and other peripherals      |
| Power Supply/Battery  | Suitable power source to drive the ESP8266 and motors                      |

**Pin Assignments for Motors (as used in the code):**

- **Right Motor**:  
  - Speed control (RIGHT_EN): GPIO14 (D5)  
  - Direction control (RIGHT_IN1): GPIO15 (D8)  
  - Direction control (RIGHT_IN2): GPIO13 (D7)

- **Left Motor**:  
  - Speed control (LEFT_EN): GPIO12 (D6)  
  - Direction control (LEFT_IN1): GPIO2 (D4)  
  - Direction control (LEFT_IN2): GPIO0 (D3)

---

## Software Design 💻📐

The firmware is written in C++ and leverages the ESP8266WiFi and ESP8266WebServer libraries. The software design is modular, separating the low-level motor operations from the higher-level control algorithms. Key functionalities include:

- **WiFi Access Point Setup**:  
  The ESP8266 is configured as an access point with the SSID `NodeMCU_Car` and an IP address provided on initialization.

- **Embedded Web Server**:  
  A lightweight HTTP server listens on port 80. Incoming HTTP requests containing motor commands (via the "State" parameter) are processed and executed.

- **Motor Control Algorithms**:  
  Various commands allow for nuanced control of the car including forward, backward, pivoting, and turning. PWM is used to adjust speeds dynamically, and custom speed levels can be set by sending numeric commands (0-9).

- **Object-Oriented Implementation**:  
  The separation into `Motor` and `MotorController` classes ensures an organized code architecture that is both scalable and easy to modify—a highly recommended practice in academic and research environments.

---

## Diagrams and Wiring 📊🔌

### System Architecture Diagram
+-------------------------------+
|         ESP8266 Module        |
|  (WiFi AP: "NodeMCU_Car")     |
+---------------+---------------+
|
| (HTTP Commands)
|
+---------------v---------------+
|  L298N Motor Driver Module  |
+---------------+---------------+
|               |                        
|               |
Right Motor     Control Signals     Left Motor
       +-------------------------------+
       |         ESP8266 Module        |
       |  (WiFi AP: "NodeMCU_Car")      |
       +---------------+---------------+
                       |
                       | (HTTP Commands)
                       |
       +---------------v---------------+
       |   L298N Motor Driver Module   |
       +---------------+---------------+
         |             |             |
         |             |             |
  Right Motor   Control Signals  Left Motor
  
### Wiring Overview

- **ESP8266 → L298N**  
  The ESP8266 GPIO pins are interlinked with the L298N's control pins according to the following configuration:  
  - Right Motor:  
    - EN: GPIO14, IN1: GPIO15, IN2: GPIO13  
  - Left Motor:  
    - EN: GPIO12, IN1: GPIO2, IN2: GPIO0

![image](https://github.com/user-attachments/assets/3797e829-b142-4a44-9a49-5f68027f4cdd)

---

## Implementation 🏗️📐

The implementation of NodeMCU Car Control is carefully structured to ensure clarity and extensibility. Here's a step-by-step breakdown:  

1. **Hardware Abstraction:**  
   - **Motor Class:**  
     Encapsulates individual motor control, handling operations such as moving forward 🟢, backward 🔙, and stopping 🛑. The class manages PWM speed control and directional signals.  
   - **MotorController Class:**  
     Integrates two motor objects ⚙️⚙️, enabling coordinated maneuvers like pivoting 🌀 and differential turns 🔄 by adjusting motor speeds.

2. **Network Initialization:**  
   - Configures the NodeMCU as a WiFi Access Point 📶, allowing remote control through any WiFi-enabled device.  
   - Launches a lightweight HTTP server 🌐 on port 80 to receive commands in real time.

3. **Command Processing Workflow:**  
   - The HTTP handler listens for simple GET requests ✉️ that include a command parameter (such as `"F"` for forward, `"B"` for backward).  
   - A dedicated function maps these commands to specific motor controller methods ✅, ensuring smooth and immediate operation.

4. **Modular & Scalable Code Design:**  
   - The clear separation of motor functions into distinct classes 🔒 ensures that additional features—like sensor integrations 🛠️ or advanced state machines 🧠—can be added with minimal disruption.  
   - This architecture supports iterative development 💻, allowing the controller to evolve alongside the challenges of competitive robotics.  

---

## Roadmap 🌟🛤️

Future plans and enhancements for the project:

- **Sensor Integration:**  
  Incorporate ultrasonic/infrared sensors for obstacle detection 👀 and avoidance 🚧.
  
- **Advanced State Machine:**  
  Develop a robust control system 💡 to manage complex sequences 🔄 and error-handling for improved reliability 🛡️.
  
- **Enhanced Web Interface:**  
  Create an interactive dashboard 🖥️ for monitoring real-time telemetry 📊 and controlling the robot 🎮.
  
- **System Scalability:**  
  Expand the design 🔝 to support additional motors ⚙️ or auxiliary modules for extra functionalities 🚀.

- **Community Collaboration:**  
  Engage with the mechatronics community 🤝 to receive feedback 📬 and collaborate on new features 🧠.

---

## License 📄🛡️

This project is licensed under the **GNU GENERAL PUBLIC LICENSE Version 3**. See the [LICENSE](LICENSE) file for further details.

---

## Inspiration 💡✨

Inspired by creative engineering showcased by Pro Now, this project embodies a relentless quest for innovation amid technical challenges ⚙️🔥. For a glimpse into the inspiration, check out the [Pro Now YouTube video](https://youtu.be/gU7vlhU1HvU?si=bU8_l4K3_jUoTEup) and dive into the world of dynamic robotics competition. 🚀🤖

---

*Let your creativity drive innovation and take your robotics game to new heights! 🌟 Happy Building! 🛠️*
