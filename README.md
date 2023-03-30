# esp_master

## Device used:

- ESP8266
- Ultrasonic Sensor - HC-SR04

## System architecture:

- Master - ESP8266:
- Receive garbage detection signal from HC-SR04 sensor.
- ESP8266 uses web socket protocol to send face recognition and garbage signals to 2 phones.
- After taking the picture, the garbage phone will send the label back to the ESP8266 and the ESP8266 will send it to the face recognition phone. Then, the face recognition phone will wait for user input signal (selecting what type of garbage), compare and finally send the signal back to the ESP8266 to know which side of the trash can the garbage is classified.
- ESP8266 communicates with Arduino Uno R3 connected to CNC Shiled V3 using the A8825 driver via the I2C data exchange protocol through the SDA and SCL ports.

## Used libraries:

- AccelStepper
- Wire
- ESP8266WiFi
- ArduinoJson
- ESP8266WiFiMulti
- WebSocketsServer
- ESP8266WebServer

## IDE Platform: PlatformIO + Arduino IDE

## Installation guide:

- For the ESP8266 board, download the code, download the appropriate libraries, and then upload the code for the Master.

## NOTE:

- When uploading code, unplug the SDA and SCL pins, and plug them in after the code is uploaded.
- Connect the motor to the board first before powering the CNC Shield V3.
