#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define ONE_WIRE_BUS 2
#define ONE_WIRE_BUS_two 3
#define ONE_WIRE_BUS_three 4

OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire2(ONE_WIRE_BUS_two);
OneWire oneWire3(ONE_WIRE_BUS_three);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);

void setup(void)
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,1);
  display.println("IoT Based");
  display.setCursor(20,22);
  display.println("Thermo");
  display.setCursor(40,43);
  display.println("Logger");
  display.display();

  sensors.begin();
  sensors2.begin();
  sensors3.begin();
}

void loop(void){
  sensors.requestTemperatures(); 
  sensors2.requestTemperatures();
  sensors3.requestTemperatures();
  delay(1500);
  float tempC = sensors.getTempCByIndex(0);
  float tempC2 = sensors2.getTempCByIndex(0);
  float tempC3 = sensors3.getTempCByIndex(0);
  Serial.print(tempC);
  Serial.print(",");
  Serial.print(tempC2);
  Serial.print(",");
  Serial.print(tempC3);
  Serial.print("\n");

  display.clearDisplay();
  display.setCursor(0,1);
  display.print("S-1: ");
  display.print(tempC);
  display.setCursor(0,22);
  display.print("S-2: ");
  display.print(tempC2);
  display.setCursor(0,43);
  display.print("S-3: ");
  display.print(tempC3);
  display.display();
}

