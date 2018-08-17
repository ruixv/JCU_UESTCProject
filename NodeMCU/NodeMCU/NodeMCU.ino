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
  This example runs directly on NodeMCU.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right NodeMCU module
  in the Tools -> Board menu!

  For advanced settings please follow ESP examples :
   - ESP8266_Standalone_Manual_IP.ino
   - ESP8266_Standalone_SmartConfig.ino
   - ESP8266_Standalone_SSL.ino

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <WEMOS_SHT3X.h>
SHT3X sht30(0x45);
#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "1cfb060debcf46a08729b25c0a405c85";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "GengRX";
char pass[] = "11111111";
MicroOLED oled(PIN_RESET, DC_JUMPER);  // I2C Example
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

}

void loop()
{
  sht30.get();
  oled.begin();     // Initialize the OLED
  oled.clear(PAGE); // Clear the display's internal memory
  oled.clear(ALL);  // Clear the library's display buffer
  oled.display();   // Display what's in the buffer (splashscreen)
  oled.setCursor(0,20);
  oled.setFontType(5);         // 7-segment font
  oled.print("AirTem ");          // Print "A0"
  oled.print(sht30.cTemp);  // Print a0 reading
  oled.setCursor(0,40);
  oled.setFontType(5);         // 7-segment font
  oled.print("AirHum: ");          // Print Air HUmidity
  oled.print(sht30.humidity);  // Print Air HUmidity Reading

  delay(100);
  Blynk.run();
}

