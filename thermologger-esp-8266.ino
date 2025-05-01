#include <ESP8266WiFi.h>

const char* ssid = "IoT-WiFI";
const char* password = "********";
const char* host = "thermal-iot.bhuiyansoft.com";
const char* api="********";

void setup() {
  Serial.begin(9600); // Communicate with Arduino Nano
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');

    WiFiClient client;
    if (client.connect(host, 80)) {
      String url = "/senddata/home/" + String(api) + "/" + String(data);
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
    }
    client.stop();
    delay(16000); // Match ThingSpeak update rate
  }
}


//https://thermal-iot.bhuiyansoft.com/senddata/home/machine_id/data
