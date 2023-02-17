// 引入 wifi 模块，并实例化，不同的芯片这里的依赖可能不同
#include <ESP8266WiFi.h>
static WiFiClient espClient;

// 引入阿里云 IoT SDK
#include <AliyunIoTSDK.h>

//引入HT11 SDK
#include "DHT.h"
#define DHTPIN  13 //D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//引入BH1750
#include <Adafruit_Sensor.h>
#include <BH1750.h>
#include <Wire.h>
BH1750 lightMeter;



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
    Serial.begin(9600);
    lightMeter.begin();
    
    // 初始化 wifi
    wifiInit(WIFI_SSID, WIFI_PASSWD);
    
    // 初始化 iot，需传入 wifi 的 client，和设备产品信息
    AliyunIoTSDK::begin(espClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET, REGION_ID);

    
    
    // 绑定一个设备属性回调，当远程修改此属性，会触发 powerCallback
    // PowerSwitch 是在设备产品中定义的物联网模型的 id
    //AliyunIoTSDK::bindData("PowerSwitch", powerCallback);
    
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

  Serial.print("AirHumidity:");
  Serial.print(AirHumidity);
  Serial.print("AirTemperature:");
  Serial.print(AirTemp);
  Serial.print("℃");
  Serial.print("Light: ");
  Serial.print(LightLux);
  Serial.println(" lx");

  AliyunIoTSDK::send("AirHumidity", AirHumidity);
  AliyunIoTSDK::send("AirTemp", AirTemp);
  AliyunIoTSDK::send("LightLux", LightLux);
  
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
//void powerCallback(JsonVariant p)
//{
//    int PowerSwitch = p["PowerSwitch"];
//    if (PowerSwitch == 1)
//    {
//        // 启动设备
//    } 
//}