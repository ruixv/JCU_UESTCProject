 #define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <WEMOS_SHT3X.h>  //添加SHT30的库

#define PIN_UPTIME1 V5  //BLYNK显示潮湿程度
#define PIN_UPTIME2 V4  //BLYNK显示摄氏度
#define PIN_UPTIME3 V3  //BLYNK显示相对湿度
#define PIN_UPTIME4 V2  //BLYNK土壤湿度值

#define Delay_Pin 0 //继电器引脚

//**************************************变量设置************************************//
int sensor_switch;  //传感器是否决定继电器的使用，0是传感器不决定继电器的使用，1是决定
int water_input;  //当传感器停止使用时，手动控制继电器的使用

//char auth[] = "1c551c6b5a8648f8ad4d2099142a1e24"; //Blynk设置
char auth[] = "1cfb060debcf46a08729b25c0a405c85";
//char ssid[] = "elequent"; //WIFI连接设置
//char pass[] = "elequent137";  //WIFI连接设置
char ssid[] = "GengRX"; //WIFI连接设置
char pass[] = "11111111";  //WIFI连接设置


float c_Temp; //摄氏度
float Relative_Humidity; //相对湿度

int Sensor_init_State;  //初始状态
int Water_init_State; //初始状态


//****************SHT-30模块设置***************//
SHT3X sht30(0x45);

//******************************湿度传感器的设置************************//
const int AirValue = 850;   //空气的湿度值
const int WaterValue = 350;  //水的湿度值
int intervals = (AirValue - WaterValue)/4;   
int soilMoistureValue = 0;
int watering;

//*********设置oled*******//
#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D
MicroOLED oled(PIN_RESET, DC_JUMPER);  // I2C Example


//********BLYNK设置*******//
BLYNK_READ(PIN_UPTIME1)
{
  //**************************手机显示潮湿程度***********************//
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME1,"Very Wet",soilMoistureValue);
  }
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME1,"Wet",soilMoistureValue);
  }
  else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME1,"Dry",soilMoistureValue);
  }
  else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME1,"Very Dry",soilMoistureValue);
  }
  
//****************手机显示温度**************//
  Blynk.virtualWrite(PIN_UPTIME2,sht30.cTemp);
  
//****************手机显示相对湿度**************//
  Blynk.virtualWrite(PIN_UPTIME3,sht30.humidity);

//****************土壤湿度值上传********************//
  Blynk.virtualWrite(PIN_UPTIME4,soilMoistureValue);
}



void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(16,OUTPUT);  //D0口输出，控制继电器
  pinMode(13,OUTPUT);  //D7口输出，控制是否使用传感器
  pinMode(15,OUTPUT);  //D8口输出，控制是否人工放水
  pinMode(12,OUTPUT); //D6口输出，控制警示灯
  oled.begin();     // Initialize the OLED
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.display();   // Display what's in the buffer (splashscreen)

  Sensor_init_State = digitalRead(13);
  Water_init_State = digitalRead(15);

  digitalWrite(13,Sensor_init_State);
  digitalWrite(15,Water_init_State);
  digitalWrite(12,LOW); //初始时警示灯处于关闭状态
  
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
//*******************SHT-30获取温度*************************//
  sht30.get();
  c_Temp = sht30.cTemp;
  Relative_Humidity = sht30.humidity;

//*******************传感器信息获取*************************//  
  sensor_switch = digitalRead(13); //用 D7 作为传感器的“开关”
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil

//*******************OLED设置*****************************//
  oled.clear(PAGE);  // Clear the buffer
  oled.setCursor(0, 0);
  oled.setFontType(12);         // 7-segment font
  
//**********************************继电器控制*************************************//
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    if(sensor_switch != 0)
    {
      digitalWrite(16,HIGH);
    }
    digitalWrite(12,LOW);
    Serial.println("Very Wet");
    oled.print("Very Wet");          // Print "A0"
  }
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))
  {
    digitalWrite(12,LOW);
    Serial.println("Wet");
    oled.print("Wet");
  }
  else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))
  {
    if(sensor_switch != 0 && c_Temp <= 30)
    {
      digitalWrite(16,LOW);
      digitalWrite(16,LOW);
    }
    else if(sensor_switch != 0 && c_Temp >30)
    {
      digitalWrite(16,HIGH); //关闭继电器
      digitalWrite(12,HIGH);  //亮灯
    }
    else if(sensor_switch ==0 && c_Temp <= 30)
    {
      digitalWrite(12,LOW);
    }
    else if(sensor_switch ==0 && c_Temp >30)
    {
      digitalWrite(12,HIGH);
    }
    Serial.println("Dry");
    oled.print("Dry");
  }
  else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))
  {
    if(sensor_switch != 0 && c_Temp <= 30)
    {
      digitalWrite(16,LOW);
      digitalWrite(12,LOW);
    }
    else if(sensor_switch != 0 && c_Temp > 30)
    {
      digitalWrite(16,HIGH); //关闭继电器
      digitalWrite(12,HIGH);  //亮灯
    }
    else if(sensor_switch ==0 && c_Temp <= 30)
    {
      digitalWrite(12,LOW);
    }
    else if(sensor_switch ==0 && c_Temp >30)
    {
      digitalWrite(12,HIGH);
    }
    Serial.println("Very Dry");
    Serial.println(digitalRead(12));
    oled.print("Very Dry");
  }

  Serial.print("Temperature in Celsius: ");
  Serial.println(c_Temp);
  Serial.print("Relative Humidity: ");
  Serial.println(Relative_Humidity);
  Serial.println();

//*******oled打印温度*********//
  oled.setCursor(0,10);
  oled.print(c_Temp);

  oled.setCursor(30,10);
  oled.print("^C");

//********oled打印相对湿度******//
  oled.setCursor(0,20);
  oled.print(Relative_Humidity);

  oled.print(30,20);
  oled.print("%");
  
//********oled打印控制状态******//
  oled.setCursor(0, 30);
  
  if(sensor_switch == 0 || (sensor_switch != 0 && c_Temp > 30.0) == 1)  //传感器不控制继电器时
  {
    water_input = digitalRead(15); //用D8作为手动放水的开关
    if(water_input == 0)
    {
      digitalWrite(16,LOW);
    }
    else
    {
      digitalWrite(16,HIGH);
    }
  }

  if(sensor_switch == 0)
  {
    oled.print("Manual!");
  }
  else
  {
    oled.print("Auto!");
  }

//********oled打印放水状态******//
  oled.setCursor(0,40);

  watering = digitalRead(16);
  if(watering != 0)
  {
    oled.print("Stop!");
  }
  else
  {
    oled.print("Irrigate!");
  }
  
  oled.display(); //  Draw the memory buffer
  
  delay(100);
  Blynk.run();
}
