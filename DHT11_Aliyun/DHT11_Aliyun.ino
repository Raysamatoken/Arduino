// 引入 wifi 模块，并实例化，不同的芯片这里的依赖可能不同
#include <ESP8266WiFi.h>
static WiFiClient espClient;

// 引入阿里云 IoT SDK
#include <AliyunIoTSDK.h>

//引入HT11 SDK
#include "DHT.h"
#define DHTPIN  14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// 设置产品和设备的信息，从阿里云设备信息里查看
#define PRODUCT_KEY "gebwuMwchQw"
#define DEVICE_NAME "TDlUAi8X0Fs57bz9kiA6"
#define DEVICE_SECRET "8a4b84d0a1cdc7f3f28b7cd8d3710d72"
#define REGION_ID "cn-shanghai"

// 设置 wifi 信息
#define WIFI_SSID "self-study room"
#define WIFI_PASSWD "buzhidao.."

void setup()
{
    dht.begin();
    Serial.begin(9600);
    
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
  float h = dht.readHumidity();//读湿度
  float t = dht.readTemperature();//读温度(摄氏度)
  
  Serial.print("Humidity:");
  Serial.print(h);
  Serial.print("% Temperature:");
  Serial.print(t);
  Serial.print(" ");
  Serial.println("℃");

  AliyunIoTSDK::send("CurrentHumidity", h);
  AliyunIoTSDK::send("CurrentTemperature", t);
  
  delay(1000);
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

// 电源属性修改的回调函数
void powerCallback(JsonVariant p)
{
    int PowerSwitch = p["PowerSwitch"];
    if (PowerSwitch == 1)
    {
        // 启动设备
    } 
}
