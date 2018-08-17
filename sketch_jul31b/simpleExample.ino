/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  This example shows how to send values to the Blynk App,
  when there is a widget, attached to the Virtual Pin and it
  is set to some frequency

  Project setup in the app:
    Value Display widget attached to V5. Set any reading
    frequency (i.e. 1 second)
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1cfb060debcf46a08729b25c0a405c85";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "GengRX";
char pass[] = "00000000";

// Use Virtual pin 5 for uptime display
#define PIN_UPTIME V5


//*************Sensor
const int AirValue = 650;   //you need to replace this value with Value_1
const int WaterValue = 350;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/4;   
int soilMoistureValue = 0;
// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)
BLYNK_READ(PIN_UPTIME)
{
  soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_UPTIME, soilMoistureValue);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
 // Blynk.begin(auth, ssid, pass, "192.168.123.37", 8080);
}

void loop()
{
  Blynk.run();
}
