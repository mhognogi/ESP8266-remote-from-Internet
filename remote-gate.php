#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

int ledPin = LED_BUILTIN; // GPIO2
int pinRel = 5; //D1

void setup() {
    USE_SERIAL.begin(9600);
    pinMode(ledPin, OUTPUT);
    pinMode(pinRel, OUTPUT);
    digitalWrite(ledPin, HIGH);
    digitalWrite(pinRel, LOW);

    const char* ssid = "wifiName";
    const char* password = "wifiPass";
    WiFiMulti.addAP(ssid, password);
}

void loop() {
 
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        http.begin("http://www.phpServer.com");
        int httpCode = http.GET();

        USE_SERIAL.println(httpCode);
        USE_SERIAL.println("______________");

        if(httpCode > 0) {
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                
                USE_SERIAL.print("_");
                USE_SERIAL.print(payload);
                USE_SERIAL.print("_");
                
                if(payload=="open"){
                    USE_SERIAL.println("poarta deschisa");
                    digitalWrite(ledPin, LOW);
                    digitalWrite(pinRel, HIGH);
                }
                else{
                    USE_SERIAL.println("poarta inchisa");
                    digitalWrite(ledPin, HIGH);
                    digitalWrite(pinRel, LOW);
                }
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(500);
    
}



