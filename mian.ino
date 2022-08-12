#include <TFT_eSPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include "DHT.h"
#include <MQ135.h>
#include<SoftwareSerial.h>
SoftwareSerial softSerial1(0, 1);
/////////////库//////////////////
int a; //通信变量
//int temp;
//int inByte;
int servoPin_1 = 4;
int servoPin_2 = 5;
int servoPin_3 = 6;
int servoPin_4 = 7;
//////舵机引脚/////
#define ONE_WIRE_BUS A2 //温度
#define DHTPIN A3   //湿度
#define PIN_MQ135 A4  //空气质量
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
void TFT_eSPI::fillScreen(uint32_t color);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
MQ135 mq135_sensor(PIN_MQ135);
#define DebugSerial Serial
/********1.8TFT彩屏**********/
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8
#define TFT_SDL  11
#define TFT_SCK  13
/********1.8TFT彩屏**********/
int oneWireData;//定义温度变量
int sb18_dat;
int h;  //定义湿度变量
int Dth;
int ppm; //定义空气质量变量
int rzero;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

/*********智能垃圾桶16进制显示*********/
const unsigned char PROGMEM str1[] =
{
  0x20, 0x00, 0x3E, 0x7C, 0x48, 0x44, 0x08, 0x44, 0xFF, 0x44, 0x14, 0x44, 0x22, 0x7C, 0x40, 0x00,
  0x1F, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x1F, 0xF0, 0x10, 0x10 /*"智",0*/
};
const unsigned char PROGMEM str2[] =
{ 0x10, 0x40, 0x24, 0x44, 0x42, 0x48, 0xFF, 0x70, 0x01, 0x40, 0x00, 0x42, 0x7E, 0x42, 0x42, 0x3E,
  0x42, 0x00, 0x7E, 0x44, 0x42, 0x48, 0x42, 0x70, 0x7E, 0x40, 0x42, 0x42, 0x4A, 0x42, 0x44, 0x3E/*"能",1*/
};
const unsigned char PROGMEM str3[] =
{ 0x10, 0x80, 0x10, 0x40, 0x10, 0x40, 0x10, 0x00, 0x13, 0xFC, 0xFC, 0x00, 0x10, 0x08, 0x11, 0x08,
  0x11, 0x08, 0x10, 0x90, 0x10, 0x90, 0x1C, 0x90, 0xE0, 0xA0, 0x40, 0x20, 0x07, 0xFE, 0x00, 0x00/*"垃",2*/
};
const unsigned char PROGMEM str4[] =
{
  0x10, 0x00, 0x17, 0xF8, 0x11, 0x08, 0x11, 0x10, 0x11, 0x10, 0xFD, 0x20, 0x11, 0x3C, 0x11, 0x04,
  0x11, 0x04, 0x12, 0x88, 0x1E, 0x88, 0xE2, 0x50, 0x44, 0x20, 0x04, 0x50, 0x08, 0x88, 0x03, 0x06 /*"圾",3*/
};
const unsigned char PROGMEM str5[] =
{
  0x20, 0x00, 0x27, 0xF8, 0x20, 0x10, 0x21, 0xA0, 0xF8, 0x40, 0x27, 0xFC, 0x24, 0x44, 0x74, 0x44,
  0x6F, 0xFC, 0xA4, 0x44, 0xA4, 0x44, 0x27, 0xFC, 0x24, 0x44, 0x24, 0x44, 0x24, 0x54, 0x24, 0x08 /*"桶",4*/
};
/*********智能垃圾桶16进制显示*********/

void setup()
{
  Serial.begin(9600);
  softSerial1.begin(9600);
  Servo_1(0);//初始化舵机角度为0度
  Servo_2(0);
  Servo_3(0);
  Servo_4(0);
  tft.initR(INITR_18GREENTAB);
  tft.setRotation(10);            //彩屏显示数据位置
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  //  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_RED);   //背景颜色设置
  sensors.begin();
  pinMode(servoPin_1, OUTPUT); //初始化舵机引脚
  pinMode(servoPin_2, OUTPUT);
  pinMode(servoPin_3, OUTPUT);
  pinMode(servoPin_4, OUTPUT);
}

void loop()
{
  while (1)
  {

    /*********汉字显示位置*********/
    tft.drawBitmap(15, 10, str1, 16, 16, ST77XX_WHITE); //画出字符对应点阵数据
    tft.drawBitmap(35, 10, str2, 16, 16, ST77XX_WHITE);//画出字符对应点阵数据
    tft.drawBitmap(55, 10, str3, 16, 16, ST77XX_WHITE); //画出字符对应点阵数据
    tft.drawBitmap(75, 10, str4, 16, 16, ST77XX_WHITE); //画出字符对应点阵数据
    tft.drawBitmap(95, 10, str5, 16, 16, ST77XX_WHITE); //画出字符对应点阵数据
    /*********汉字显示位置*********/
    temperature1();
    DHT1();
    MQ();
    tft_sensor();
    Dht();
    avelible();

  }
}

void temperature1() //温度初始化获取温度
{
  tempreture();
  sensors.requestTemperatures(); // 发送命令获取温度
  sb18_dat = sensors.getTempCByIndex(0);
  if (sb18_dat != DEVICE_DISCONNECTED_C) //如果获取到的温度正常
  {
    oneWireData = sb18_dat;
    //    Serial.print("当前温度是:");
    //    Serial.print(oneWireData,DEC);
    //    Serial.println("℃\n");
    delay(500);
  }
}
void DHT1() //湿度初始化获取湿度
{
  Humi();
  h = dht.readHumidity();
  Dth = h;
  Serial.print("当前湿度是:");
  Serial.print(Dth);
  Serial.println("f\n");
}
void MQ() //空气质量初始化获取值
{
  QM13();
  ppm = mq135_sensor.getPPM();
  rzero = ppm;
  //  Serial.println(ppm);
  //  delay(1000);
}

void tft_sensor() //tft屏显示
{

  if (oneWireData != 0 || rzero != 0)
  {
    //    tempreture();
    tft.setCursor(40, 60);
    tft.setTextColor(ST77XX_BLACK);//用背景色去写上次的数据，相当于擦除字
    tft.print("");
    tft.print(sb18_dat);
    tft.println(" C");


    tft.setCursor(40, 60);
    delay(500);
    tft.setTextColor(ST77XX_RED);
    tft.print("");
    tft.print(oneWireData);
    tft.println(" C");

    //    tft.setCursor(1, 40);
    //    tft.setTextColor(ST77XX_BLACK);//用背景色去写上次的数据，相当于擦除字
    //    tft.print("Hmui:");
    //    tft.print(h);
    //    tft.println(" F");
    //
    //
    //    tft.setCursor(1, 40);
    //    delay(1000);
    //    tft.setTextColor(ST77XX_RED);
    //    tft.print("Hmui:");
    //    tft.print(Dth);
    //    tft.println(" F");

    //    QM13();
    tft.setCursor(60, 80);
    tft.setTextColor(ST77XX_BLACK);//用背景色去写上次的数据，相当于擦除字
    tft.print("");
    tft.print(ppm);
    tft.println("pm");


    tft.setCursor(60, 80);
    delay(500);
    tft.setTextColor(ST77XX_RED);
    tft.print("");
    tft.print(rzero);
    tft.println("pm");
  }
  //  if (Dth != 0)
  //  {
  //    Humi();
  //    tft.setCursor(35, 40);
  //    tft.setTextColor(ST77XX_BLACK);//用背景色去写上次的数据，相当于擦除字
  //    tft.print("");
  //    tft.print(h);
  //    tft.println(" F");
  //
  //
  //    tft.setCursor(35, 40);
  //    delay(1000);
  //    tft.setTextColor(ST77XX_RED);
  //    tft.print("");
  //    tft.print(Dth);
  //    tft.println(" F");
  //  }
  /*********彩屏显示温度*********/
  sb18_dat = oneWireData;
  h = Dth;
  ppm = rzero;
  delay(100);
}
void Dht() //湿度显示
{
  if (Dth != 0)
  {

    tft.setCursor(35, 40);
    tft.setTextColor(ST77XX_BLACK);//用背景色去写上次的数据，相当于擦除字
    tft.print("");
    tft.print(h);
    tft.println(" F");


    tft.setCursor(35, 40);
    delay(500);
    tft.setTextColor(ST77XX_RED);
    tft.print("");
    tft.print(Dth);
    tft.println(" F");
  }
  h = Dth;
  delay(100);
}
void avelible() //语音模块通信获取值
{
  softSerial1.listen();
  a = softSerial1.read();
  switch (a)
  {
    case 1:   //其他垃圾
      tft.setCursor(10, 60);
      other();
      Servo_1(80);
      delay(3000);
      tft.fillScreen(TFT_RED);
      Servo_1(0);
      delay(5);
      break;

    case 2:  //可回收垃圾
      tft.setCursor(10, 60);
      recyclables();
      Servo_2(80);
      delay(3000);
      tft.fillScreen(TFT_RED);
      Servo_2(0);
      delay(5);
      break;

    case 3: //有害垃圾
      tft.setCursor(10, 60);
      harmful();
      Servo_3(80);
      delay(3000);
      tft.fillScreen(TFT_RED);
      Servo_3(0);
      delay(5);
      break;

    case 4:  //厨余垃圾
      tft.setCursor(10, 60);
      kitchen();
      Servo_4(80);
      delay(3000);
      tft.fillScreen(TFT_RED);
      Servo_4(0);
      delay(5);
      break;
  }
}
///////////舵机控制变量函数/////////////////////
void Servo_1(int angle_1)
{
  for (int i = 0; i < 50; i++)
  {
    int pulsewidth = (angle_1 * 11) + 500;
    digitalWrite(servoPin_1, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin_1, LOW);
    delayMicroseconds(20000 - pulsewidth);
  }
  delay(1000);
}
void Servo_2(int angle_2)
{
  for (int i = 0; i < 50; i++)
  {
    int pulsewidth = (angle_2 * 11) + 500;
    digitalWrite(servoPin_2, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin_2, LOW);
    delayMicroseconds(20000 - pulsewidth);
  }
  delay(1000);
}
void Servo_3(int angle_3)
{
  for (int i = 0; i < 50; i++)
  {
    int pulsewidth = (angle_3 * 11) + 500;
    digitalWrite(servoPin_3, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin_3, LOW);
    delayMicroseconds(20000 - pulsewidth);
  }
  delay(1000);
}
void Servo_4(int angle_4)
{
  for (int i = 0; i < 50; i++)
  {
    int pulsewidth = (angle_4 * 11) + 500;
    digitalWrite(servoPin_4, HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(servoPin_4, LOW);
    delayMicroseconds(20000 - pulsewidth);
  }
  delay(1000);
}
///////////舵机控制变量函数/////////////////////
