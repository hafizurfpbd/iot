#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ======= WiFi Credentials =======
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// ======= BhuiyanSoft API ========
const char* server = "http://thermologger.bhuiyansoft.com/update";
String apiKey = "******";

// ======= DHT11 Setup ============
#define DHTPIN 16
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ======= MQ-135 Setup ===========
#define MQ135_PIN 36 // ADC pin

// ======= DS18B20 Setup ==========
#define ONE_WIRE_BUS_one 15
#define ONE_WIRE_BUS_two 2
#define ONE_WIRE_BUS_three 4
OneWire oneWire1(ONE_WIRE_BUS_one);
OneWire oneWire2(ONE_WIRE_BUS_two);
OneWire oneWire3(ONE_WIRE_BUS_three);
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);

// ========== Setup ===============
void setup() {
  Serial.begin(115200);
  dht.begin();
  sensors1.begin();
  sensors2.begin();
  sensors3.begin();
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

// ========== Main Loop ==========
void loop() {
  float tempRoom = dht.readTemperature();
  float hum = dht.readHumidity();
  int airQuality = analogRead(MQ135_PIN);

  sensors1.requestTemperatures();
  sensors2.requestTemperatures();
  sensors3.requestTemperatures();
  delay(2000);
  float freezerTemp1 = sensors1.getTempCByIndex(0);
  float freezerTemp2 = sensors2.getTempCByIndex(0);
  float freezerTemp3 = sensors3.getTempCByIndex(0);

  Serial.printf("Room Temp: %.1f°C, Humidity: %.1f%%, Air: %d, Freezer-1: %.2f°C, Freezer-2: %.2f°C, Freezer-3: %.2f°C\n",
                tempRoom, hum, airQuality, freezerTemp1,freezerTemp2,freezerTemp3);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    client.setInsecure();
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey +
                 "&tempRoom=" + String(tempRoom) +
                 "&hum=" + String(hum) +
                 "&airQuality=" + String(airQuality) +
                 "&freezerTemp1=" + String(freezerTemp1)+
                 "&freezerTemp2=" + String(freezerTemp2)+
                 "&freezerTemp3=" + String(freezerTemp3);
    
    http.begin(client,url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
     WiFi.begin(ssid, password);
  }

  delay(15000); 
}
