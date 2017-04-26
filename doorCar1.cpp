#include <ESP8266WiFi.h>

//sudo chmod a+rw /dev/ttyUSB0

const char* ssid     = "***";
const char* password = "***";

WiFiServer server(80);

int ledPin = D4;
int openPin = D1;
int closePin = D3;
int signalingPin = D5;
int alarmPin = D7;

String statusDoor = "open";
int maxTryCon = 7;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

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

  int tryCon = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    if (tryCon >= maxTryCon && statusDoor == "open") {
      closeDoor();
      statusDoor == "close";
    }
  }

  server.begin();
  Serial.println("");
  Serial.println(WiFi.localIP());

}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  String request = client.readString();
  //Serial.println(request);
  client.flush();

  if (request.indexOf("openClose=close") != -1) {
    digitalWrite(ledPin, HIGH);
    closeDoor();
    statusDoor = "close";
  }
  if (request.indexOf("openClose=open") != -1) {
    digitalWrite(ledPin, LOW);
    openDoor();
    statusDoor = "open";
  }

  // Return the response
  client.println("<html>");
  client.println("  <head>");
  client.println("    <meta name='viewport' content='width=device-width, user-scalable=no'>");
  client.println("    <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>");
  client.println("    <script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js'></script>");
  client.println("    <script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>");
  if (statusDoor == "open") {
    client.println("    <link href='data:image/x-icon;base64,AAABAAEAEBAQAAAAAAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAAVFRTAAEJCgC6uroALoWPAE/J1gA+p7MACy8zAB5fZgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAd3d3dwAAAAeEZVZIcAAAB4RiJkhwAAAHhGImSHAAAAeEYiZIcAAAB4QiIkhwAAAHhCIiSHAAAAeEYiZIcAAAB4RlVkhwAAAAd3d3dwAREAAAAAExABMQAAAAATEAExAAAAABMRETEAAAAAATMzEAAAAAAAEREAD//wAAwD8AAIAfAACAHwAAgB8AAIAfAACAHwAAgB8AAIAfAACAHwAAwDEAAP4xAAD+MQAA/gEAAP8DAAD/hwAA' rel='icon' type='image/x-icon' />");
  }
  if (statusDoor == "close") {
    client.println("    <link href='data:image/x-icon;base64,AAABAAEAEBAQAAEABAAoAQAAFgAAACgAAAAQAAAAIAAAAAEABAAAAAAAgAAAAAAAAAAAAAAAEAAAAAAAAAAAAAAABgYGAACSuABmZmYAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAIiIiIiIiAAAiIiIiIiIAACIiIRIiIgAAIiIhEiIiAAAiIiESIiIAACIiEREiIgAAIiIRESIiAAAiIiESIiIAACIiIiIiIgAAIiIiIiIiAAAzAAAAADMAADMAAAAAMwAAAzMAADMwAAAAMzMzMwAAAAADMzMwAAD//wAAwAMAAMADAADAAwAAwAMAAMADAADAAwAAwAMAAMADAADAAwAAwAMAAM/zAADP8wAA48cAAPAPAAD4HwAA' rel='icon' type='image/x-icon' />");
  }
  client.println("  </head>");
  client.println("  <body>");

  if (statusDoor == "open") {
    client.println("  <div class='alert alert-warning' style='font-size: 20px; text-align: center'>The door is <span style='font-size: 25px'>Open</span></div>");
  } else if (statusDoor == "close") {
    client.println("  <div class='alert alert-success' style='font-size: 20px; text-align: center'>The door is <span style='font-size: 25px'>Close</span></div>");
  } else {
    client.print("    Door is undefined");
  }

  client.println("    <form class='container' action='' method='post' style='margin-top:20px'>");
  client.println("      <input type='hidden' name='openClose' value='open'>");
  client.println("      <button type='submit' class='btn btn-warning btn-lg btn-block'><h4>Open the door</h4></button>");
  client.println("    </form>");

  client.println("    <form class='container' action='' method='post' style='margin-top:20px'>");
  client.println("      <input type='hidden' name='openClose' value='close'>");
  client.println("      <button type='submit' class='btn btn-success btn-lg btn-block'><h4>Close the door</h4></button>");
  client.println("    </form>"); 

  client.println("  </body>");
  client.println("</html>");

  Serial.println(statusDoor);
}

void closeDoor() {
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
  Serial.println("Inchide usa");
}

void openDoor() {
  digitalWrite(ledPin, LOW);
  digitalWrite(openPin, LOW);
  digitalWrite(signalingPin, LOW);
  digitalWrite(alarmPin, LOW);
  delay(500);
  digitalWrite(ledPin, HIGH);
  digitalWrite(openPin, HIGH);
  digitalWrite(signalingPin, HIGH);
  digitalWrite(alarmPin, HIGH);
  Serial.println("Deschide usa");
}
