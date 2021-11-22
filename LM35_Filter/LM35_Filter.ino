/*
 * LM35
 * 模拟温度传感器LM35测试
 */
int potPin = A0;
float temperature = 0;
long value = 0;
float Filter_Value;

float ds18b20_temp = 0;
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
  value = analogRead(potPin); 
  //0.00488125=5/1024，0~5V对应模拟口读数1~1024，每10毫伏对应1℃
  temperature = ((value * 5.0 * 100)/1024-1);

  sensors.requestTemperatures(); // 发送命令获取温度
  ds18b20_temp = sensors.getTempCByIndex(0)-1;
  
  Filter_Value = Filter();       // 获得滤波器输出值
  
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(ds18b20_temp);
  Serial.print(",");
  Serial.print(Filter_Value+0.5); // 串口输出
  Serial.println(",");
  delay(100);
}

float Get_AD() {
  return temperature;
}

// 中位值平均滤波法（又称防脉冲干扰平均滤波法）（算法1）
#define FILTER_N 100
float Filter() {
  int i, j;
  float filter_temp, filter_sum = 0;
  int filter_buf[FILTER_N];
  for(i = 0; i < FILTER_N; i++) {
    filter_buf[i] = Get_AD();
    delay(1);
  }
  // 采样值从小到大排列（冒泡法）
  for(j = 0; j < FILTER_N - 1; j++) {
    for(i = 0; i < FILTER_N - 1 - j; i++) {
      if(filter_buf[i] > filter_buf[i + 1]) {
        filter_temp = filter_buf[i];
        filter_buf[i] = filter_buf[i + 1];
        filter_buf[i + 1] = filter_temp;
      }
    }
  }
  // 去除最大最小极值后求平均
  for(i = 1; i < FILTER_N - 1; i++) filter_sum += filter_buf[i];
  return filter_sum / (FILTER_N - 2);
}

// 中位值滤波法
//#define FILTER_N 100
//float Filter() {
//  int filter_buf[FILTER_N];
//  int i, j;
//  float filter_temp;
//  for(i = 0; i < FILTER_N; i++) {
//    filter_buf[i] = Get_AD();
//  }
//  // 采样值从小到大排列（冒泡法）
//  for(j = 0; j < FILTER_N - 1; j++) {
//    for(i = 0; i < FILTER_N - 1 - j; i++) {
//      if(filter_buf[i] > filter_buf[i + 1]) {
//        filter_temp = filter_buf[i];
//        filter_buf[i] = filter_buf[i + 1];
//        filter_buf[i + 1] = filter_temp;
//      }
//    }
//  }
//  return filter_buf[(FILTER_N - 1) / 2];
//}

//算术平均滤波法
//#define FILTER_N 12
//float Filter() {
//  int i;
//  float filter_sum = 0;
//  for(i = 0; i < FILTER_N; i++) {
//    filter_sum += Get_AD();
//    delay(1);
//  }
//  return (float)(filter_sum / FILTER_N);
//}

//递推平均滤波法（又称滑动平均滤波法）

//#define FILTER_N 12
//int filter_buf[FILTER_N + 1];
//float Filter() {
//  int i;
//  float filter_sum = 0;
//  filter_buf[FILTER_N] = Get_AD();
//  for(i = 0; i < FILTER_N; i++) {
//    filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，低位仍掉
//    filter_sum += filter_buf[i];
//  }
//  return (float)(filter_sum / FILTER_N);
//}
