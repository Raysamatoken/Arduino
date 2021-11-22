#include <SimpleKalmanFilter.h>

/*
 This sample code demonstrates how to use the SimpleKalmanFilter object. 
 Use a potentiometer in Analog input A0 as a source for the reference real value.
 Some random noise will be generated over this value and used as a measured value.
 The estimated value obtained from SimpleKalmanFilter should match the real
 reference value.

 SimpleKalmanFilter(e_mea, e_est, q);
 e_mea: Measurement Uncertainty 
 e_est: Estimation Uncertainty 
 q: Process Noise
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
 
SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 10;
long refresh_time;

void setup() {
  Serial.begin(9600);
  sensors.begin();
}

void loop() {

  value = analogRead(potPin); 
  //0.00488125=5/1024，0~5V对应模拟口读数1~1024，每10毫伏对应1℃
  temperature = ((value * 5.0 * 100)/1024-0.7);

  sensors.requestTemperatures(); // 发送命令获取温度
  ds18b20_temp = sensors.getTempCByIndex(0)-1;

  // read a reference value from A0 and map it from 0 to 100
  float real_value = ds18b20_temp;
  
  // add a noise to the reference value and use as the measured value
  float measured_value = temperature;

  // calculate the estimated value with Kalman Filter
  float estimated_value = simpleKalmanFilter.updateEstimate(measured_value);

  // send to Serial output every 100ms
  // use the Serial Ploter for a good visualization
  if (millis() > refresh_time) {
    Serial.print(measured_value);
    Serial.print(",");
    Serial.print(real_value);
    Serial.print(",");
    Serial.print(estimated_value);
    Serial.println(",");
    
    refresh_time = millis() + SERIAL_REFRESH_TIME;
  }

}
