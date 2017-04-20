#include <ESP8266WiFi.h>

const char* ssid     = "***";
const char* password = "***";
int lampPin = 2; // D4 led
//int openPin = 4; //D2
int openPin = 5; //D1
int closePin = 0; //D3

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  String statusDoor = "open";
  int signalMinim = -45;

  pinMode(lampPin, OUTPUT);
  pinMode(openPin, OUTPUT);
  pinMode(closePin, OUTPUT);
  digitalWrite(lampPin, HIGH);
  digitalWrite(openPin, HIGH);
  digitalWrite(closePin, HIGH);

  while (true) {
    if (WiFi.status() != WL_CONNECTED) {
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        statusDoor = closeDoor(statusDoor);
      }
    }

    if (WiFi.RSSI() > signalMinim) {
      statusDoor = openDoor(statusDoor);
    } else {
      statusDoor = closeDoor(statusDoor);
    }

    Serial.print(statusDoor);
    Serial.println(WiFi.RSSI());

    delay(1000);
  }
}

String closeDoor(String statusDoor) {
  if (statusDoor == "open") {
    digitalWrite(lampPin, LOW);
    digitalWrite(closePin, LOW);
    delay(500);
    digitalWrite(lampPin, HIGH);
    digitalWrite(closePin, HIGH);
    delay(500);
    digitalWrite(lampPin, LOW);
    delay(500);
    digitalWrite(lampPin, HIGH);
    Serial.println("Inchide usa");
  }

  return "close";
}

String openDoor(String statusDoor) {
  if (statusDoor == "close") {
    digitalWrite(lampPin, LOW);
    digitalWrite(openPin, LOW);
    delay(500);
    digitalWrite(lampPin, HIGH);
    digitalWrite(openPin, HIGH);
    Serial.println("Deschide usa");
  }

  return "open";
}

void loop() {
}
