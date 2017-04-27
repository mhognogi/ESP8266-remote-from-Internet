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
infile:///home/marius/Downloads/CV_2017_ro.docxt maxTryCon = 7;

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
    String src = "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAABHNCSVQICAgIfAhkiAAAAllJREFUSIljYKAxYCRXIy8Pj6CbKl+EqxKHh5YQg+TbX8zfd979dnjL9Y/znrz9co8iC5y0JH2mWf+ep876RhRd7tN/zj+td2Um9+y5W/Lv379/zKQabqMu4brG9t06OdbP/Njk2Rn/MLkIvrNkFFUSPXD7/TaSfMDGxsZ+Mob/hgHHawUGBgaGT//Y/+74KHng5NPvJyX4ueRDxV+7K7B9EWFgYGD49p/1r+kWfjuSXO+kLR3xL5Ph//8shv/vMth/2mjJ+SPLC/Bwip2KF77xPwuiZoqfzGomUixwkfhuzwj187TbfIuOXHu0EVn+w5fvrzKOspf9/w/h+ykxe5BkgboQsyqMffYD52lsaq48erPz3T+OfwwMDAwMn1/ykGTB79+/4eq/svBhVfPr16+f/9l5/zMwMDBIsv5gYCHFgt2v+Ta8Zxa6y8DAwPDgxbuzOBVCw5GFiYGBcXUA/055lo+CpFgEA32PZGetOPV4Drr463yxPyK/XzEzMDAwsIRIfXQjx3AGBgYG8+9/L65gYMCwABmQFETEguLT3EV8LLLsfxmYcVvw8g/Xl867EjNEOBkliuUeRLMz/CU6Uy46dn8SjI3Tgq7bolP799+rYGBgYFCKkFeIEHpoQ6wFyABnMhXhZICnw7//YFmHdIDTB4WKL9M3q4iuY2JiFggWfkqW6/FawPH/B5MIl4DYD0Z2cQ5G4sMfHZCUk8kBeJPpBOOPs5kZKXMEXguUOL9zUWI4AwMDA9Off5RoJxw1LL2vtJdpi3NokGr039+/f6+8/XodWe6iJgAARja/38+HtpsAAAAASUVORK5CYII=";
    client.println("  <div class='alert alert-warning' style='font-size: 20px; text-align: center'>The door is <span style='font-size: 25px'>Open</span> <img src='" + src + "' style='margin-top: -10px'></div>");
  } else if (statusDoor == "close") {
    client.println("  <div class='alert alert-success' style='font-size: 20px; text-align: center'>The door is <span style='font-size: 25px'>Close</span> <span class='glyphicon glyphicon-lock'></span></div>");
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
