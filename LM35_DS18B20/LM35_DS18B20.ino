int potPin = A0;
float lm35_temperature = 0;
float ds18b20_temp = 0;
long value = 0;

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {
  Serial.begin(9600); //设置串口波特率9600
   sensors.begin();
}

void loop() {
  value = analogRead(potPin); //读取模拟输入

  //电压与摄氏度转换：
  //0.00488125=5/1024，0~5V对应模拟口读数1~1024，每10毫伏对应1℃
  lm35_temperature = ((value * 5.0 * 100)/1024);

  sensors.requestTemperatures(); // 发送命令获取温度
  ds18b20_temp = sensors.getTempCByIndex(0);
  
  Serial.print(lm35_temperature);
  Serial.print(" ");
  Serial.print(ds18b20_temp);
  Serial.print(" ");
  delay(1000);

}
