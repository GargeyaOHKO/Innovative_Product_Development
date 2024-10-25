// Define the motor control pins
const int motorPin1 = 8;
const int motorPin2 = 9;
const int enablePin = 10;
// Define the speed of the motor (0 to 255)
int motorSpeed = 150; // 50% speed

void setup() {
  // Set the motor control pins as outputs
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);

  // Start with the motor off
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  analogWrite(enablePin, 0);
}

void loop() {
  // Set the motor direction
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);

  // Control the motor speed
  analogWrite(enablePin, motorSpeed);
  
  // Wait for 2 seconds
  delay(2000);
  
  // Turn the motor off
  // analogWrite(enablePin, 0);
  
  // Optional: Wait for some time before running the motor again
  // delay(5000);
}