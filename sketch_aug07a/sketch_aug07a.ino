#include <WEMOS_SHT3X.h>
#include <Blynk.h>

#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
SHT3X sht30(0x45);
//bool status_ = HIGH;
char auth[] = "b8f5570995bb43e1b1aae2ab9705f921";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "GengRX";
char pass[] = "11111111";
//const int relayPin = D1;
BLYNK_READ(V5)
{
  Blynk.virtualWrite(V5,sht30.cTemp);
/*  Blynk.virtualWrite(PIN_UPTIME7,sht30.cTemp);
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))    // Very Wet
  {
    Blynk.virtualWrite(PIN_UPTIME,"Very Wet",soilMoistureValue);
  }
  else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))   // Wet
  {
    Blynk.virtualWrite(PIN_UPTIME,"Wet",soilMoistureValue);
  }
  else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))   // Dry
  {
    Blynk.virtualWrite(PIN_UPTIME,"Dry",soilMoistureValue);
  }
  else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))   // Very Dry
  {
    Blynk.virtualWrite(PIN_UPTIME,"Very Dry",soilMoistureValue);
  }*/
}
void setup() {
  Serial.begin(115200);
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
    Blynk.begin(auth, ssid, pass);
}
 
void loop() {
  sht30.get();
  
  Serial.print("Temperature in Celsius : ");
  Serial.println(sht30.cTemp);
  Serial.print("Temperature in Fahrenheit : ");
  Serial.println(sht30.fTemp);
  
  Serial.print("Relative Humidity : ");
  Serial.println(sht30.humidity);
  Serial.println();
  if (sht30.cTemp > 30)
  {
        digitalWrite(D7, HIGH);
        digitalWrite(D6, LOW);
  }
else if(sht30.cTemp <29)
  {
        digitalWrite(D7, LOW);
        digitalWrite(D6, HIGH);
  }
  else if ((sht30.cTemp < 30) && (sht30.cTemp > 29))
  {
        digitalWrite(D7, LOW);
        digitalWrite(D6, LOW);
  }
  delay(1000);
    Blynk.run();
}
