/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 14     // Digital pin connected to the DHT sensor
#define ONE_WIRE_BUS 2 // 数据输出脚接开发板数字引脚4
// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();
  display.setTextColor(WHITE);
}

void loop() {
  delay(1000);

  Serial.print("Temperature for the DHT11: ");
  Serial.println(dht.readTemperature()); 

  Serial.print("Humidity for the DHT11: ");
  Serial.println(dht.readHumidity()); 
  
  Serial.print("Temperature for D18B20: ");
  Serial.println(sensors.getTempCByIndex(0)); 
  
  //read temperature and humidity
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float t2 = sensors.getTempCByIndex(0);
  if (isnan(h) || isnan(t) ||isnan(t2)) {
    Serial.println("Failed to read from  sensor!");
  }
  // clear display
  display.clearDisplay();
  
  // display temperature
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("DHT11: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(t);
  display.print("C");
  
  // display humidity
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.print(h);
  display.print("%"); 

  // display D18B20
  display.setTextSize(1);
  display.setCursor(80,0);
  display.print("D18B20: ");
  display.setTextSize(1);
  display.setCursor(80, 10);
  display.print(t2);
  display.print("C"); 
  
  
  
  display.display(); 
}
