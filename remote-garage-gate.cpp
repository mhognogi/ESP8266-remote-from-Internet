#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define Serial Serial


int ledPin = LED_BUILTIN; // GPIO2
int pinRelOpen = 5; //D1
int pinRelClose = 0; //D3

void setup() {
    Serial.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(pinRelOpen, OUTPUT);
    pinMode(pinRelClose, OUTPUT);
    digitalWrite(ledPin, HIGH);
    digitalWrite(pinRelOpen, LOW);
    digitalWrite(pinRelClose, LOW);

    const char* ssid = "wifi name";
    const char* password = "wifi pass";
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
        delay(500);
        Serial.println("Waiting for connection");
    }
}

void loop() {
    if (WiFi.status () == WL_CONNECTED){
        HTTPClient http;
        http.begin("http://www.phpServer.test");
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        int httpCode = http.POST("user=myusername&keyq=hashkey");
        if(httpCode > 199 && httpCode < 300) {
            String response = http.getString();
            if(response == "open"){
                Serial.println("open gate");
                digitalWrite(ledPin, LOW);
                digitalWrite(pinRelOpen, HIGH);
                delay(5000);
                digitalWrite(ledPin, HIGH);
                digitalWrite(pinRelOpen, LOW);
            } else if(response=="close"){
                Serial.println("close gate");
                digitalWrite(ledPin, LOW);
                digitalWrite(pinRelClose, HIGH);
                delay(5000);
                digitalWrite(ledPin, HIGH);
                digitalWrite(pinRelOpen, LOW);
            } else {
                Serial.println("Silence is gold");
            }
        }
        http.end();
    } else{
        Serial.println("No response from server");
    }
    delay(1000);
}
