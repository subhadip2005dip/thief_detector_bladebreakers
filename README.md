# Smart Cupboard Protection System

An IoT-based security system for monitoring and protecting cupboard contents using ESP8266, ultrasonic sensor, and IR sensor with Blynk integration.

## Features

- Real-time motion detection using ultrasonic sensor
- Object presence monitoring using IR sensor
- LED status indicators (Red/Green)
- Mobile notifications via Blynk app
- Distance measurement and tracking
- Theft detection and alert system

## Hardware Requirements

- NodeMCU ESP8266 board
- HC-SR04 Ultrasonic sensor
- IR sensor module
- 2x LEDs (Red and Green)
- Resistors for LEDs
- Jumper wires
- Power supply
- Breadboard

## Pin Configuration

- Ultrasonic Trigger: D1
- Ultrasonic Echo: D2
- IR Sensor: D5
- Red LED: D6
- Green LED: D7

## Software Requirements

- Arduino IDE
- Blynk IoT app
- Required Libraries:
  - ESP8266WiFi
  - BlynkSimpleEsp8266

## Setup Instructions

1. Install required libraries in Arduino IDE
2. Configure WiFi credentials in the code
3. Set up Blynk app:
   - Create new project
   - Use the provided template ID and auth token
   - Add widgets for distance, IR status, and alerts
4. Upload code to ESP8266
5. Connect sensors according to pin configuration
6. Power up the system

## How It Works

- System continuously monitors distance and object presence
- Green LED indicates normal operation
- Red LED activates during potential theft detection
- Alerts are sent to Blynk app when:
  - Someone approaches the cupboard (distance < 10cm)
  - Object is removed from IR sensor
  - Both conditions occur simultaneously (potential theft)

## Blynk Dashboard Setup

Virtual Pins:
- V0: Distance measurement display
- V1: IR sensor status indicator
- V2: Alert status display

## Contributing

Feel free to contribute to this project by submitting issues or pull requests.

## License

This project is open-source and available under the MIT License.
