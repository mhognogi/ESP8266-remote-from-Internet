#include <ESP8266WiFi.h>

const char* ssid     = "***";
const char* password = "***";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  String statusDoor = "open"; 
  int signalMinim = -50;

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

    delay(1000);
  }
}

String closeDoor(String statusDoor) {
  if (statusDoor == "open") {
    Serial.println("Inchide usa");
  }

  return "close";
}

String openDoor(String statusDoor) {
  if (statusDoor == "close") {
    Serial.println("Deschide usa");
  }

  return "open";
}

void loop() {
}
