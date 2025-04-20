#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// === Pin Definitions for Motors (using L298N module) ===
#define RIGHT_EN    14  // Right motor speed control (GPIO14, D5)
#define RIGHT_IN1   15  // Right motor direction control (GPIO15, D8)
#define RIGHT_IN2   13  // Right motor direction control (GPIO13, D7)

#define LEFT_EN     12  // Left motor speed control (GPIO12, D6)
#define LEFT_IN1    2   // Left motor direction control (GPIO2, D4)
#define LEFT_IN2    0   // Left motor direction control (GPIO0, D3)

// Access Point SSID
const char* ssid = "NodeMCU_Car";

// Create a web server on port 80
ESP8266WebServer server(80);

// === Motor Class: Manage an individual motor ===
class Motor {
  private:
    uint8_t enPin;
    uint8_t in1Pin;
    uint8_t in2Pin;
  
  public:
    Motor(uint8_t en, uint8_t in1, uint8_t in2) 
      : enPin(en), in1Pin(in1), in2Pin(in2) {}

    void init() {
      pinMode(enPin, OUTPUT);
      pinMode(in1Pin, OUTPUT);
      pinMode(in2Pin, OUTPUT);
    }

    // Move forward at a specified speed (PWM value 0-1023)
    void forward(int speed) {
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, HIGH);
      analogWrite(enPin, speed);
    }

    // Move backward at a specified speed
    void backward(int speed) {
      digitalWrite(in1Pin, HIGH);
      digitalWrite(in2Pin, LOW);
      analogWrite(enPin, speed);
    }

    // Stop the motor
    void stop() {
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, LOW);
      analogWrite(enPin, 0);
    }
};

// === MotorController Class: Controls both motors together ===
class MotorController {
  private:
    Motor &rightMotor;
    Motor &leftMotor;
    int baseSpeed;      // Base speed (PWM)
    int speedCoeff;     // Reduction factor for turning
  
  public:
    MotorController(Motor &r, Motor &l, int speed = 800, int coeff = 3)
      : rightMotor(r), leftMotor(l), baseSpeed(speed), speedCoeff(coeff) {}

    void init() {
      rightMotor.init();
      leftMotor.init();
    }

    // Move forward straight
    void forward() {
      rightMotor.forward(baseSpeed);
      leftMotor.forward(baseSpeed);
    }

    // Move backward straight
    void backward() {
      rightMotor.backward(baseSpeed);
      leftMotor.backward(baseSpeed);
    }

    // Pivot in place to the left
    void pivotLeft() {
      rightMotor.forward(baseSpeed);
      leftMotor.backward(baseSpeed);
    }

    // Pivot in place to the right
    void pivotRight() {
      rightMotor.backward(baseSpeed);
      leftMotor.forward(baseSpeed);
    }

    // Turn left while moving forward (reduces left motor speed)
    void forwardLeft() {
      rightMotor.forward(baseSpeed);
      leftMotor.forward(baseSpeed / speedCoeff);
    }

    // Turn right while moving forward (reduces right motor speed)
    void forwardRight() {
      rightMotor.forward(baseSpeed / speedCoeff);
      leftMotor.forward(baseSpeed);
    }

    // Turn left while moving backward (reduces left motor speed)
    void backwardLeft() {
      rightMotor.backward(baseSpeed);
      leftMotor.backward(baseSpeed / speedCoeff);
    }

    // Turn right while moving backward (reduces right motor speed)
    void backwardRight() {
      rightMotor.backward(baseSpeed / speedCoeff);
      leftMotor.backward(baseSpeed);
    }

    // Stop both motors
    void stop() {
      rightMotor.stop();
      leftMotor.stop();
    }

    // Adjust the base speed using a level (0 through 9)
    void adjustSpeed(int level) {
      // Mapping: level 0 => 400, level 9 => 1023
      int increment = (1023 - 400) / 9;
      baseSpeed = 400 + level * increment;
      Serial.print("Adjusted baseSpeed: ");
      Serial.println(baseSpeed);
    }
};

// === Instantiate Motor Objects and MotorController ===
Motor motorRight(RIGHT_EN, RIGHT_IN1, RIGHT_IN2);
Motor motorLeft(LEFT_EN, LEFT_IN1, LEFT_IN2);
MotorController controller(motorRight, motorLeft);

// === Function to process incoming commands ===
void processCommand(String cmd) {
  if (cmd == "F") {
    controller.forward();
  } 
  else if (cmd == "B") {
    controller.backward();
  } 
  else if (cmd == "L") {
    controller.pivotLeft();
  } 
  else if (cmd == "R") {
    controller.pivotRight();
  }
  else if (cmd == "I") {   // Slight right turn while moving forward
    controller.forwardRight();
  } 
  else if (cmd == "G") {   // Slight left turn while moving forward
    controller.forwardLeft();
  } 
  else if (cmd == "J") {   // Right turn while moving backward
    controller.backwardRight();
  } 
  else if (cmd == "H") {   // Left turn while moving backward
    controller.backwardLeft();
  } 
  else if (cmd == "S") {
    controller.stop();
  } 
  else if (cmd.length() == 1 && isDigit(cmd.charAt(0))) {
    int level = cmd.toInt(); // level between 0 and 9
    controller.adjustSpeed(level);
  }
}

// === HTTP Handler to receive commands ===
void handleRoot() {
  if (server.hasArg("State")) {
    String cmd = server.arg("State");
    Serial.println("Received Command: " + cmd);
    processCommand(cmd);
  }
  server.send(200, "text/plain", "OK");
}

// === Setup: Initializes the system, WiFi AP, and web server ===
void setup() {
  Serial.begin(115200);
  controller.init();

  // Configure WiFi as an Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Setup Web Server routes
  server.on("/", handleRoot);
  server.onNotFound(handleRoot);
  server.begin();
  Serial.println("HTTP server started.");
}

// === Main Loop: Handle incoming client requests ===
void loop() {
  server.handleClient();
  // Additional area: Sensor monitoring or status logging can be added here.
}
