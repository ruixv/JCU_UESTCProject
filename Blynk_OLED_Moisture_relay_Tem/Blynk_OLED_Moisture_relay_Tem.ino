/*************************************************************
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <WEMOS_SHT3X.h>

SHT3X sht30(0x45);
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1cfb060debcf46a08729b25c0a405c85";               //授权码
char ssid[] = "GengRX";                                         //WifiSSID
char pass[] = "11111111";                                       //Wifi密码

// Use Virtual pin 5 for uptime display
#define PIN_UPTIME V5                             
#define PIN_UPTIME6 V6
#define PIN_UPTIME7 V7
#define PIN_UPTIME8 V8
#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D
#define ONE_WIRE_BUS 2 

const int AirValue = 850;   //you need to replace this value with Value_1
const int WaterValue = 350;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/4;   
int soilMoistureValue = 0;
int DangerousTem =35;
// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)
MicroOLED oled(PIN_RESET, DC_JUMPER);  // I2C Example

BLYNK_READ(PIN_UPTIME)
{
  Blynk.virtualWrite(PIN_UPTIME6,sht30.humidity);
  Blynk.virtualWrite(PIN_UPTIME7,sht30.cTemp);
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_UPTIME8,soilMoistureValue);
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))    // Very Wet
  {
    Blynk.virtualWrite(PIN_UPTIME,"Very Wet");
  }
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))   // Wet
  {
    Blynk.virtualWrite(PIN_UPTIME,"Wet");
  }
  else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))   // Dry
  {
    Blynk.virtualWrite(PIN_UPTIME,"Dry");
  }
  else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))   // Very Dry
  {
    Blynk.virtualWrite(PIN_UPTIME,soilMoistureValue);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(16,OUTPUT);  //D0口输出，控制电机
  pinMode(13,OUTPUT);  //D7口输出，由APP控制，切换手动/自动切换
  pinMode(15,OUTPUT);  //D8口输出，由APP控制，手动模式下切换是/否浇水
  pinMode(12,OUTPUT);  //D6口输出, 控制警报灯
  oled.begin();     // Initialize the OLED
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.display();   // Display what's in the buffer (splashscreen)
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  sht30.get();
  Serial.print("Temperature in Celsius:");
  Serial.println(sht30.cTemp);
  Serial.print("Temperature in Fahrenheit : ");
  Serial.println(sht30.fTemp);
  Serial.print("Relative Humidity : ");
  Serial.println(sht30.humidity);
  Serial.println();
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  oled.clear(PAGE);  // Clear the buffer
  oled.setCursor(0, 0);           // 控制显示位置
  oled.print("SoiHum ");          // Print "A0"
  oled.print(soilMoistureValue);  // Print a0 reading
  oled.setCursor(0, 10);
  digitalWrite(12, LOW);
  if(digitalRead(15) == HIGH)
  {
    if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))    // Very Wet
    {
      digitalWrite(16,HIGH);
      Serial.println("Very Wet");
      oled.print("Very Wet");          // Print "A0"
    }
    else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))
    {
//      digitalWrite(16,HIGH);       //两个阈值之间留一定距离，防止震荡
      Serial.println("Wet");
      oled.print("Wet");
    }
    else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))
    {
      if(sht30.cTemp < DangerousTem)
      {
        digitalWrite(16,LOW);
      }
      else if(sht30.cTemp >= DangerousTem)
      {
        digitalWrite(12,HIGH);  
      }
      Serial.println("Dry");
      oled.print("Dry");
    }
    else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))
    {
      if(sht30.cTemp < DangerousTem)
      {
        digitalWrite(16,LOW);
      }
      else if(sht30.cTemp >= DangerousTem)
      {
        digitalWrite(12,HIGH);  
      }
      Serial.println("Very Dry");
      oled.print("Very Dry");
    }
  }
  
  else if(digitalRead(15) == LOW)
  {
    if(digitalRead(13) == HIGH)
    {
      digitalWrite(16,LOW);
      Serial.println("Open");
      oled.print("Hand"); 
    }
    else if(digitalRead(13) == LOW)
    {
      digitalWrite(16,HIGH);
      Serial.println("Hand-Mode Close");
      oled.print("Hand"); 
     }
  }
  oled.setCursor(0,20);
  oled.setFontType(5);         // 7-segment font
  oled.print("AirTem ");          // Print "A0"
  oled.print(sht30.cTemp);  // Print a0 reading
  oled.setCursor(0,40);
  oled.setFontType(5);         // 7-segment font
  oled.print("AirHum: ");          // Print Air HUmidity
  oled.print(sht30.humidity);  // Print Air HUmidity Reading


  oled.display(); //  Draw the memory buffer

  delay(100);
  Blynk.run();
}
