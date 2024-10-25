// #include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>

// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// #define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
// #define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)

// void setup() {
//   Serial.begin(9600);
//   Serial.println("PCA9685 Servo Control Test!");

//   pwm.begin();
//   pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

//   Wire.begin();
// }

// void loop() {
//   if (Serial.available()) {
//     int servoPos = Serial.parseInt();  // Read the integer from serial
//     if (servoPos<10) {
//       int pulseLength = map(75, 0, 180, SERVOMIN, SERVOMAX);
//       pwm.setPWM(1, 0, pulseLength);
//       delay(4000*servoPos);
//       pulseLength = map(78, 0, 180, SERVOMIN, SERVOMAX);
//       pwm.setPWM(1, 0, pulseLength);
//     }
//   }
// }
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // Minimum pulse length count (out of 4096)
#define SERVOMAX  600 // Maximum pulse length count (out of 4096)

void setup() {
  Serial.begin(9600);
  Serial.println("PCA9685 Servo Control Test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  Wire.begin();
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
        Serial.print(" moved ");
        Serial.print(moveTime);
        Serial.println(" times.");
      } else {
        Serial.println("Invalid time input! Please enter a positive integer.");
      }
    } else {
      Serial.println("Invalid servo number! Enter a value between 0 and 15.");
    }
  }
}
