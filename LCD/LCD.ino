#include <LiquidCrystal_I2C.h> //引用I2C库
#include <Wire.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
void setup() {
lcd.init(); // 初始化LCD
lcd.backlight(); //设置LCD背景等亮
lcd.setCursor(0,0); //设置显示指针
lcd.print("Welcome to "); //输出字符到LCD1602上
lcd.setCursor(0,1);
lcd.print("It's my system");
delay(2000);
lcd.clear();
}

void loop() {
// put your main code here, to run repeatedly:
lcd.setCursor(0,0);
lcd.print("hello");
lcd.setCursor(0,1);
lcd.print("world");
}
