// 引入 wifi 模块，并实例化，不同的芯片这里的依赖可能不同
#include <ESP8266WiFi.h>
static WiFiClient espClient;

// 引入阿里云 IoT SDK
#include <AliyunIoTSDK.h>

//引入DHT11 SDK
#include "DHT.h"
#define DHTPIN  13 //D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//引入BH1750
#include <Adafruit_Sensor.h>
#include <BH1750.h>
#include <Wire.h>
BH1750 lightMeter;

//土壤湿度传感器YL-69

#define PIN_AO A0   //土壤传感器AO接ESP8266引脚A0
//#define PIN_DO 4  //湿度高于设定值时，DO输出高电平，模块提示灯亮
 
int M0 = 1024;  //在空气中AO读取的值最大为1024，代表干燥时的读数
int M1 = 464;   //浸泡在水里的最小值 464（最小值会改变），代表100%湿度

// 设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY "gebwLenoMHo"
#define DEVICE_NAME "MonitorMCU"
#define DEVICE_SECRET "038479b3bbbdb8fdc3e91a3dc5c26da5"
#define REGION_ID "cn-shanghai"

// 设置 wifi 信息
#define WIFI_SSID "SweetHome_5G"
#define WIFI_PASSWD "loveliang0506"

void setup()
{
    dht.begin();
    Wire.begin(D4,D3);//D3--SCL  D4--SDA
    pinMode(PIN_AO, INPUT);//土壤湿度传感器A0口
    // pinMode(PIN_DO, INPUT);//土壤湿度传感器D0口
    lightMeter.begin();//光照传感器
    Serial.begin(9600);
    
    // 初始化 wifi
    wifiInit(WIFI_SSID, WIFI_PASSWD);
    
    // 初始化 iot，需传入 wifi 的 client，和设备产品信息
    AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

    
    
    // 绑定一个设备属性回调，当远程修改此属性，会触发 powerCallback
    // PowerSwitch 是在设备产品中定义的物联网模型的 id
    //AliyunIoTSDK::bindData("PowerSwitch", powerCallback);
    AliyunIoTSDK::bindData("FanPowerSwitch", powerCallback);
    
    // 发送一个数据到云平台
    //AliyunIoTSDK::send("CurrentHumidity", h);
    //AliyunIoTSDK::send("CurrentTemperature", t);
}

void loop()
{
  AliyunIoTSDK::loop();
  float AirHumidity = dht.readHumidity();//读湿度
  float AirTemp = dht.readTemperature();//读温度(摄氏度)

  float LightLux = lightMeter.readLightLevel();

  float SoilHumidity = (M0 - analogRead(PIN_AO)) / ((M0 - M1) / 100); //将土壤湿度模拟量转换成数字量

  float SoilTemp = AirTemp + 1.2;

  Serial.print("AirHumidity:");
  Serial.print(AirHumidity);
  Serial.println("%");
  
  Serial.print("AirTemperature:");
  Serial.print(AirTemp);
  Serial.println("℃");
  
  Serial.print("Light: ");
  Serial.print(LightLux);
  Serial.println(" lx");

  Serial.print("SoilHumidity= ");
  Serial.print(SoilHumidity);
  Serial.println("% ");

  Serial.print("SoilTemperature= ");
  Serial.print(SoilTemp);
  Serial.println("℃");

  
  // Serial.print("DO=");
  // Serial.println(digitalRead(PIN_DO));

  AliyunIoTSDK::send("AirHumidity", AirHumidity);
  AliyunIoTSDK::send("AirTemp", AirTemp);
  AliyunIoTSDK::send("LightLux", LightLux);
  AliyunIoTSDK::send("SoilHumidity", SoilHumidity);
  AliyunIoTSDK::send("SoilTemp", SoilTemp);
  
  delay(10000);
}

// 初始化 wifi 连接
void wifiInit(const char *ssid, const char *passphrase)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passphrase);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("WiFi not Connect");
    }
    Serial.println("Connected to AP");
}

//// 电源属性修改的回调函数
void powerCallback(JsonVariant p)
{
    int FanPowerSwitch = p["FanPowerSwitch"];
    if (FanPowerSwitch == 1)
    {
        // 启动设备
    } 
}
