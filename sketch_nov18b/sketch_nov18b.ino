/*
A、名称：算术平均滤波法
B、方法：
    连续取N个采样值进行算术平均运算：
    N值较大时：信号平滑度较高，但灵敏度较低；
    N值较小时：信号平滑度较低，但灵敏度较高；
    N值的选取：一般流量，N=12；压力：N=4。
C、优点：
    适用于对一般具有随机干扰的信号进行滤波；
    这种信号的特点是有一个平均值，信号在某一数值范围附近上下波动。
D、缺点：
    对于测量速度较慢或要求数据计算速度较快的实时控制不适用；
    比较浪费RAM。
E、整理：shenhaiyu 2013-11-01
*/

float ds18b20_temp = 0;
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

 
float Filter_Value;
 
void setup() {
  Serial.begin(9600);       // 初始化串口通信
  randomSeed(analogRead(0)); // 产生随机种子
  sensors.begin();
}
 
void loop() {

  sensors.requestTemperatures(); // 发送命令获取温度
  ds18b20_temp = sensors.getTempCByIndex(0)+2.4;

  Filter_Value = Filter();       // 获得滤波器输出值
  Serial.print((Get_AD())/10); // 串口输出
  Serial.print(",");
  Serial.print(ds18b20_temp); // 串口输出
  Serial.print(",");
  Serial.print(Filter_Value);
  Serial.println(",");
  delay(50);
}
 
// 用于随机产生一个300左右的当前值
float Get_AD() {
  return ((random(10, 17))+195-(random(10, 17)));
}
 
// 算术平均滤波法
//#define FILTER_N 12
//float Filter() {
//  int i;
//  float filter_sum = 0;
//  for(i = 0; i < FILTER_N; i++) {
//    filter_sum += (Get_AD())/10;
//    delay(1);
//  }
//  return (float)(filter_sum / FILTER_N);
//}

#define FILTER_N 12
int filter_buf[FILTER_N + 1];
float Filter() {
  int i;
  float filter_sum = 0;
  filter_buf[FILTER_N] = Get_AD()/10;
  for(i = 0; i < FILTER_N; i++) {
    filter_buf[i] = filter_buf[i + 1]; // 所有数据左移，低位仍掉
    filter_sum += filter_buf[i];
  }
  return (float)(filter_sum / FILTER_N);
}
