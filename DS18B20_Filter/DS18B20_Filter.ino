#include <OneWire.h>
#include <DallasTemperature.h>

// 数据输出脚接开发板数字引脚2
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
}

void loop(void)
{ 
  sensors.requestTemperatures(); // 发送命令获取温度
  //Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.println(",");
  delay(100); 
}
