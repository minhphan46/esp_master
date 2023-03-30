#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <AccelStepper.h>

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
LiquidCrystal_I2C lcd(0x27,16,2); 

#define TRIGGER D5
#define ECHO D6

const char* ssid = "<YOUR_SSID>";
const char* password = "<YOUR_PASSWORD>";

// Define static IP Settings
// IPAddress local_IP(192, 168, 1, 202);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);
// IPAddress primaryDNS(8, 8, 8, 8);
// IPAddress secondaryDNS(8, 8, 4, 4);

// StaticJsonDocument<500> TempDoc;
String message = "";
unsigned long t = 0;
String cmd = "";
int numberOfMobile = 0;
char data ='n';
bool status = 1;


void objectDetection();
void turnMotorLeft();
void turnMotorRight();

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      numberOfMobile--;
      if (numberOfMobile < 0) {
      numberOfMobile = 0;
      }
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Mobile: ");
      lcd.setCursor(10,0);
      lcd.setCursor(10, 0);
      lcd.print(numberOfMobile);
      break;

    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        numberOfMobile++;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Mobile: ");
        lcd.setCursor(10,0);
        lcd.setCursor(10, 0);
        lcd.print(numberOfMobile);
        // send message to client
        webSocket.sendTXT(num, "0");
      }
      break;

    case WStype_TEXT:
      // Serial.printf("[%u] get Text: %s\n", num, payload);
      // send message to client
      // webSocket.sendTXT(num, "message here");
      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      cmd ="";
      for (int i = 0; i < length; i++) {
        cmd = cmd + (char) payload[i];
      } //merging payload to single string

      Serial.println("Data from flutter:");
      
      // if (cmd == "add") {
      //   message = "capture";
      //   // Serial.println(message);
        
      //   // TempDoc["signal"] = message;
      //   // char msg[256];
      //   //serializeJson(TempDoc, msg);
      //   webSocket.broadcastTXT(message);
      // }  else 
      if (cmd == "left") {
        Serial.println(cmd + ": LEFT");
        turnMotorLeft();
      } else if (cmd =="right") {
        Serial.println(cmd + ": RIGHT");
        turnMotorRight();
      } else {
        message = cmd;
        Serial.println(message);
        webSocket.broadcastTXT(message);
      }
      break;
      
    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      // send message to client
      // webSocket.sendBIN(num, payload, length);
      break;
  }

}

String toString(const IPAddress& address) {
  return String() + address[0] + "." + address[1] + "." + address[2] + "." + address[3];
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(115200);
  // Wire.begin();
  delay(1000);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  WiFiMulti.addAP(ssid, password);
  // if(!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure!");    
  // }
  lcd.print("Connecting...");
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  lcd.clear();
  lcd.setCursor(3,0);
  lcd.print("Connected!");
  lcd.setCursor(2, 1);
  Serial.println(WiFi.localIP());
  String localIp = toString(WiFi.localIP());
  lcd.print(localIp);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  server.begin();

  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
  server.handleClient();
  objectDetection();
}

void turnMotorLeft() {
  data = 'l';
  Wire.beginTransmission(2);
  Wire.write(data);
  Wire.endTransmission();
  data = 'n';
  delay(200);
  Wire.beginTransmission(2);
  Wire.write(data);
  Wire.endTransmission();
  Serial.println("END");
}

void turnMotorRight() {
  data = 'r';
  Wire.beginTransmission(2);
  Wire.write(data);
  Wire.endTransmission();
  data = 'n';
  delay(200);
  Wire.beginTransmission(2);
  Wire.write(data);
  Wire.endTransmission();
  Serial.println("END");
}

void objectDetection() {
  long duration;
  int distance;
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = duration * 0.034 / 2;
  delay(200);
  if (distance < 15) {
    if (status) {
      // Serial.println("OKKKKKKKKKKKKKKKKKKK");
      webSocket.broadcastTXT("capture");
      status = 0;
    } else {
      if (cmd == "left" || cmd == "right") {
        status = 1;
        cmd = "";
      }
    }
  }
  // } else {
  //   Serial.println("No object");
  // }
}
