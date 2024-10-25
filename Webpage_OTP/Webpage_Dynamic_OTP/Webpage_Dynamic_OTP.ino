#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Keypad.h>
#include <ArduinoJson.h>

// Network credentials
const char* ssid = "Obi-WLAN Kenobi";
const char* password = "micromax2";

// Create PCA9685 object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  150  // Minimum pulse length count (out of 4096)
#define SERVOMAX  600  // Maximum pulse length count (out of 4096)
// Setup AsyncWebServer
AsyncWebServer server(80);

// Variables for OTP
String generatedOTP;
unsigned long otpGeneratedTime;  // Store the time when the OTP was generated (for expiration)

// Servo variables
String receivedServos[9];  // Adjust size based on your use case

// Keypad setup
const byte ROWS = 4; 
const byte COLS = 4; 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {25, 26, 27, 14};  // Connect to keypad row pins
byte colPins[COLS] = {12, 13, 32, 33};  // Connect to keypad column pins

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  // Setup PCA9685
  pwm.begin();
  pwm.setPWMFreq(50);  // Frequency for servos (50Hz)
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());
  Serial.println("Connected to WiFi");

  // Start server and listen for JSON request
  server.on("/generate_otp", HTTP_GET, [](AsyncWebServerRequest *request){
    // Generate a random 4-digit OTP
    generatedOTP = String(random(1000, 9999));
    otpGeneratedTime = millis();  // Store the time when OTP was generated
    
    // Send the OTP as a JSON response
    String response = "{\"otp\": \"" + generatedOTP + "\"}";
    request->send(200, "application/json", response);
    
    Serial.println("Generated OTP: " + generatedOTP);
  });

  server.on("/move_servos", HTTP_POST, [](AsyncWebServerRequest *request){
    // Parse the JSON
    String body = request->arg("plain");  // This gets the plain body of the POST request
    Serial.println("Received JSON:");
    Serial.println(body);  // Print the received JSON to the serial monitor

    DynamicJsonDocument doc(512);  // Adjust size as per your JSON structure
    DeserializationError error = deserializeJson(doc, body);  // Parse the JSON

    if (error) {
      Serial.print(F("Failed to parse JSON: "));
      Serial.println(error.c_str());  // Print any deserialization error
      request->send(400, "application/json", "{\"status\": \"Invalid JSON\"}");
      return;
    }

    // If parsing is successful, continue with processing
    JsonArray servos = doc["servos"];
    String otp = doc["otp"];

    // Print parsed data to the serial monitor for debugging
    Serial.print("OTP: ");
    Serial.println(otp);
    
    Serial.println("Servo Numbers: ");
    for (int i = 0; i < servos.size(); i++) {
        Serial.print("Servo ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(servos[i].as<String>());  // Print each servo number
    }

    // Check OTP validity
    if (otp == generatedOTP && millis() - otpGeneratedTime < 300000) {
      for (int i = 0; i < servos.size(); i++) {
        receivedServos[i] = servos[i].as<String>();
      }
      request->send(200, "application/json", "{\"status\": \"OTP verified, servos moving\"}");
      Serial.println("OTP Verified, moving servos...");
      moveServos();
    } else {
      request->send(401, "application/json", "{\"status\": \"Invalid or expired OTP\"}");
      Serial.println("Invalid or expired OTP");
    }
});

  server.begin();
  Serial.println("Server started, listening for requests...");

}

void moveServos() {
  // Move servos based on the received JSON data
  for (int i = 0; i < 9; i++) {
    if (receivedServos[i] != "") {
        int servoNumber = receivedServos[i].toInt();
        Serial.print("Moving Servo Number: ");
        Serial.println(servoNumber);

        int pulseLength = map(75, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(servoNumber, 0, pulseLength);
        delay(2800);

        pulseLength = map(78, 0, 180, SERVOMIN, SERVOMAX);
        pwm.setPWM(servoNumber, 0, pulseLength);

        Serial.println("Servo Movement Done!");
    }
  }
}

void loop() {
  static String enteredOTP = "";
  char key = keypad.getKey();
  if (key != NO_KEY) {
    Serial.print("Key Pressed: ");
    Serial.println(key);  // For debugging

    if (key == '#') {
      // OTP completion check
      Serial.print("Entered OTP: ");
      Serial.println(enteredOTP);
      
      if (enteredOTP.equals(generatedOTP) && millis() - otpGeneratedTime < 300000) {
        Serial.println("OTP Verified via keypad. Moving Servos.");
        moveServos();
      } else {
        Serial.println("Invalid or expired OTP via keypad.");
      }
      enteredOTP = "";  // Clear the OTP after verification
    } else {
      enteredOTP += key;  // Append the entered key to the OTP
    }
  }
}
