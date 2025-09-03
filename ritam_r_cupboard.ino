#define BLYNK_TEMPLATE_ID "TMPL3TyvU-uxb"
#define BLYNK_TEMPLATE_NAME "ESP8266 Notifications"
#define BLYNK_AUTH_TOKEN "UTrwia-fZ1t7ueoFAbjmO05p-0YlOT8q"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Define sensor pins
#define TRIGGER_PIN D1
#define ECHO_PIN D2
#define IR_SENSOR_PIN D5
#define RED_LED D6
#define GREEN_LED D7

// Ultrasonic sensor configuration
#define MAX_DISTANCE 30 // Maximum distance to check (in cm)

// Authentication details for Blynk
char auth[] = BLYNK_AUTH_TOKEN; // Replace with your Blynk auth token
char ssid[] = "SUDIPTA SAHA";        // Replace with your WiFi SSID
char pass[] = "bolbo naa";    // Replace with your WiFi password

// Variables for sensor readings
int distance;
bool irDetected;
bool ultrasonicTriggered;
bool alertActive = false;
bool someoneApproaching = false;
bool objectLifted = false;

// Thresholds
int ULTRASONIC_THRESHOLD = 10; // Distance in cm to trigger ultrasonic sensor

// Blynk virtual pins
#define VPIN_DISTANCE V0    // Label widget to show distance in cm
#define VPIN_IR_STATUS V1   // LED widget that lights up when value = 1
#define VPIN_ALERT_STATUS V2 // Value display for alert status

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  
  // Start with green LED (all safe)
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  
  // Connect to WiFi and Blynk
  Blynk.begin(auth, ssid, pass);
  Serial.println("Connecting to Blynk...");
  
  // Initialize Blynk values
  Blynk.virtualWrite(VPIN_DISTANCE, 0);
  Blynk.virtualWrite(VPIN_IR_STATUS, 0);
  Blynk.virtualWrite(VPIN_ALERT_STATUS, 0);
  
  Serial.println("Smart Cupboard Protector Initialized");
  Serial.println("System is active and monitoring...");
}

void loop() {
  Blynk.run(); // Run Blynk background processes
  
  // Read ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2; // Calculate distance in cm
  
  if (distance <= 0 || distance > MAX_DISTANCE) {
    distance = MAX_DISTANCE; // Handle out of range readings
  }
  
  // Check if someone is approaching
  ultrasonicTriggered = (distance > 0 && distance < ULTRASONIC_THRESHOLD);
  
  // Read IR sensor (LOW means object detected, HIGH means object removed)
  irDetected = digitalRead(IR_SENSOR_PIN) == HIGH;
  
  // Send sensor data to Blynk for monitoring
  Blynk.virtualWrite(VPIN_DISTANCE, distance);
  Blynk.virtualWrite(VPIN_IR_STATUS, irDetected ? 1 : 0);
  
  // Ultrasonic sensor messages
  if (ultrasonicTriggered && !someoneApproaching) {
    Serial.println("Someone is approaching the cupboard!");
    someoneApproaching = true;
  } else if (!ultrasonicTriggered && someoneApproaching) {
    Serial.println("Person has moved away from cupboard.");
    someoneApproaching = false;
  }
  
  // IR sensor messages
  if (irDetected && !objectLifted) {
    Serial.println("Churi Hoye Gechee!!!");
    objectLifted = true;
  } else if (!irDetected && objectLifted) {
    Serial.println("Object has been returned to shelf.");
    objectLifted = false;
  }
  
  // Check if BOTH conditions are met (hand in range AND object lifted)
  if (ultrasonicTriggered && irDetected) {
    // THEFT DETECTED: Hand in range AND object lifted
    digitalWrite(GREEN_LED, LOW);    // Turn off green LED
    digitalWrite(RED_LED, HIGH);     // Turn on red LED
    
    // Update Blynk alert status
    if (!alertActive) {
      Blynk.virtualWrite(VPIN_ALERT_STATUS, 1); // Set alert status to active
      alertActive = true;
      Serial.println("🚨 ALERT: Theft in progress! 🚨");
    }
  } else {
    // ANY OTHER SCENARIO: Green LED always lit
    digitalWrite(GREEN_LED, HIGH);   // Turn on green LED
    digitalWrite(RED_LED, LOW);      // Turn off red LED
    
    // Update Blynk alert status if it was previously active
    if (alertActive) {
      Blynk.virtualWrite(VPIN_ALERT_STATUS, 0); // Reset alert status
      alertActive = false;
      Serial.println("Alert cleared. All safe now.");
    }
  }
  
  // Display distance information
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Small delay to stabilize readings
  delay(1000);
}