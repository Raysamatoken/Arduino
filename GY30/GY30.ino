#include <BH1750.h>
#include <Wire.h>


BH1750 lightMeter;

void setup(){

  Serial.begin(115200);
  Wire.begin(D4,D3);//D3--SCL  D4--SDA

  lightMeter.begin();
  Serial.println(F("BH1750 Test"));

}

void loop() {

  float lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

}
