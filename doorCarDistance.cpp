#include <ESP8266WiFi.h>

//sudo chmod a+rw /dev/ttyUSB0

const char* ssid     = "***";
const char* password = "***";
int signalMinim = -80;
int ledPin = 2; // D4 led
int openPin = 5; //D1
int closePin = 0; //D3
int signalingPin = 14; //D5
int alarmPin = 13; //D7

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  String statusDoor = "open";

  pinMode(ledPin, OUTPUT);
  pinMode(openPin, OUTPUT);
  pinMode(closePin, OUTPUT);
  pinMode(signalingPin, OUTPUT);
  pinMode(alarmPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(openPin, HIGH);
  digitalWrite(closePin, HIGH);
  digitalWrite(signalingPin, HIGH);
  digitalWrite(alarmPin, HIGH);

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
    digitalWrite(ledPin, LOW);
    digitalWrite(closePin, LOW);
    digitalWrite(signalingPin, LOW);
    digitalWrite(alarmPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    digitalWrite(closePin, HIGH);
    digitalWrite(signalingPin, HIGH);
    digitalWrite(alarmPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    digitalWrite(signalingPin, LOW);
    digitalWrite(alarmPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    digitalWrite(signalingPin, HIGH);
    digitalWrite(alarmPin, HIGH);
    Serial.println("close the door");
  }

  return "close";
}

String openDoor(String statusDoor) {
  if (statusDoor == "close") {
    digitalWrite(ledPin, LOW);
    digitalWrite(openPin, LOW);
    digitalWrite(signalingPin, LOW);
    digitalWrite(alarmPin, LOW);
    delay(500);
    digitalWrite(ledPin, HIGH);
    digitalWrite(openPin, HIGH);
    digitalWrite(signalingPin, HIGH);
    digitalWrite(alarmPin, HIGH);
    Serial.println("Open the door");
  }

  return "open";
}

void loop() {
}
