#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Keypad.h>

// PCA9685 object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Define servo limits
#define SERVOMIN  150  // Minimum pulse length count (out of 4096)
#define SERVOMAX  600  // Maximum pulse length count (out of 4096)

// Keypad settings
const byte ROWS = 4;  // Four rows
const byte COLS = 4;  // Four columns
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connect row pins (R1, R2, R3, R4) to GPIO pins 25, 26, 27, 14
byte rowPins[ROWS] = {25, 26, 27, 14}; 

// Connect column pins (C1, C2, C3, C4) to GPIO pins 12, 13, 32, 33
byte colPins[COLS] = {12, 13, 32, 33}; 

// Create Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int servoNum = -1;  // Variable to store servo number
int moveTime = 0;   // Variable to store movement time
bool awaitingInput = true;

void setup() {
  Serial.begin(9600);
  Wire.begin(21, 22);  // SDA = GPIO 21, SCL = GPIO 22 (ESP32 default I2C pins)
  
  // Initialize the PCA9685
  pwm.begin();
  pwm.setPWMFreq(60);  // Set PWM frequency to 60 Hz for servos

  Serial.println("Enter Servo number (0-15), then time (1-9):");
}

void loop() {
  char key = keypad.getKey();  // Get keypress from the keypad

  if (key) {  // Check if a key was pressed
    if (key >= '0' && key <= '9') {
      int num = key - '0';  // Convert char to int

      if (awaitingInput) {
        servoNum = num;  // First input is the servo number
        awaitingInput = false;
        Serial.print("Selected Servo: ");
        Serial.println(servoNum);
      } else {
        moveTime = num;  // Second input is the time
        Serial.print("Moving Servo ");
        Serial.print(servoNum);
        Serial.print(" for ");
        Serial.print(moveTime);
        Serial.println(" seconds");

        // Move the servo to 75 degrees
        int pulseLength = map(75, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(servoNum, 0, pulseLength);
        delay(moveTime * 2800);

        // Move the servo to 78 degrees
        pulseLength = map(78, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(servoNum, 0, pulseLength);

        // Reset input state
        awaitingInput = true;
        Serial.println("Done! Enter next servo number.");
      }
    } else if (key == '*') {
      // Reset inputs if '*' is pressed
      awaitingInput = true;
      servoNum = -1;
      moveTime = 0;
      Serial.println("Input reset. Enter servo number again.");
    }
  }
}
