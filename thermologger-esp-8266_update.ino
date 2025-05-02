#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "xxxxxx";
const char* password = "********";
const char* host = "https://thermal-iot.bhuiyansoft.com/";
const char* api = "********";

unsigned long lasttime = 0;
const unsigned long interval = 10000;

void setup() {
  Serial.begin(9600);
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('E');
    unsigned long currenttime = millis();
    if (currenttime - lasttime >= interval) {
      lasttime = currenttime;

      if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient http;

        http.begin(client, host+"/" + api +"/" + data);
        int httpCode = http.GET();
        if (httpCode > 0) {
          String response = http.getString();
          Serial.println("Response: " + response);
        } else {
          Serial.println("GET failed, error: " + http.errorToString(httpCode));
        }
        http.end();

      } else {
        WiFi.begin(ssid, password);
      }
    }
  }
}
