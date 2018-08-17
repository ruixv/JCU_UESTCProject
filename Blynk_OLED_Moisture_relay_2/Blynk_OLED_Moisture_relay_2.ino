/*************************************************************
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1cfb060debcf46a08729b25c0a405c85";
char ssid[] = "elequent";
char pass[] = "elequent137";

// Use Virtual pin 5 for uptime display
#define PIN_UPTIME V5
#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

const int AirValue = 850;   //you need to replace this value with Value_1
const int WaterValue = 350;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/4;   
int soilMoistureValue = 0;
// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)
MicroOLED oled(PIN_RESET, DC_JUMPER);  // I2C Example
BLYNK_READ(PIN_UPTIME)
{
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME,"Very Wet",soilMoistureValue);
  }
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME,"Wet",soilMoistureValue);
  }
  else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME,"Dry",soilMoistureValue);
  }
  else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))
  {
    Blynk.virtualWrite(PIN_UPTIME,"Very Dry",soilMoistureValue);
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(0,OUTPUT);  //D3口输出
  pinMode(13,OUTPUT);  //D7口输出
  pinMode(15,OUTPUT);  //D8口输出
  oled.begin();     // Initialize the OLED
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.display();   // Display what's in the buffer (splashscreen)
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  oled.clear(PAGE);  // Clear the buffer
  oled.setCursor(0, 0);
  oled.print("Humidity index: ");          // Print "A0"
  oled.setFontType(12);         // 7-segment font
  oled.print(soilMoistureValue);  // Print a0 reading
  oled.setCursor(0, 20);

  if(digitalRead(15) == HIGH)
  {
    if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
    {
      digitalWrite(0,HIGH);
      Serial.println("Very Wet");
      oled.print("Very Wet");          // Print "A0"
    }
    else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))
    {
      digitalWrite(0,HIGH);
      Serial.println("Wet");
      oled.print("Wet");
    }
    else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))
    {
      digitalWrite(0,LOW);
      Serial.println("Dry");
      oled.print("Dry");
      
    }
    else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))
    {
      digitalWrite(0,LOW);
      Serial.println("Very Dry");
      oled.print("Very Dry");
    }
  }
  
  else if(digitalRead(15) == LOW)
  {
    if(digitalRead(13) == HIGH)
    {
      digitalWrite(0,LOW);
      Serial.println("Open");
      oled.print("Hand-Mode Open"); 
    }
    else if(digitalRead(13) == LOW)
    {
      digitalWrite(0,HIGH);
      Serial.println("Hand-Mode Close");
      oled.print("Hand-Mode Close"); 
     }
  }


  oled.display(); //  Draw the memory buffer
  delay(100);
  Blynk.run();
}
