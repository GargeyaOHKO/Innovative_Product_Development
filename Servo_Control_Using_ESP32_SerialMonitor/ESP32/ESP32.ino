#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create an instance of the PCA9685 PWM driver
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Define the servo pulse length range (for typical servos)
#define SERVOMIN  150  // Minimum pulse length count (out of 4096)
#define SERVOMAX  600  // Maximum pulse length count (out of 4096)

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);
  Serial.println("PCA9685 Servo Control Test on ESP32!");

  // Initialize I2C with ESP32-specific pins for SDA and SCL
  Wire.begin(21, 22);  // SDA = GPIO 21, SCL = GPIO 22 (ESP32 default I2C pins)

  // Initialize the PCA9685
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos typically run at ~60 Hz

  // Small delay to allow I2C communication setup
  delay(10);
}

void loop() {
  if (Serial.available()) {
    // First input: Servo number
    int servoNum = Serial.parseInt();  // Read the servo number
    if (servoNum >= 0 && servoNum < 16) {  // Validating servo number (0 to 15)

      // Second input: Time (in seconds)
      while (!Serial.available()) {
        // Wait for the next input
      }
      int moveTime = Serial.parseInt();  // Read the time input

      if (moveTime > 0) {  // Validate the time
        // Move the servo to position 75 degrees
        int pulseLength = map(75, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(servoNum, 0, pulseLength);
        delay(4000 * moveTime);  // Delay based on time input

        // Move the servo to position 78 degrees
        pulseLength = map(78, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(servoNum, 0, pulseLength);

        Serial.print("Servo ");
        Serial.print(servoNum);
        Serial.print(" moved for ");
        Serial.print(moveTime);
        Serial.println(" seconds.");
      } else {
        Serial.println("Invalid time input! Please enter a positive integer.");
      }
    } else {
      Serial.println("Invalid servo number! Enter a value between 0 and 15.");
    }
  }
}
