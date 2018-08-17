/***************************************************
 This example reads Capacitive Soil Moisture Sensor.
 
 Created 2015-10-21
 By berinie Chen <bernie.chen@dfrobot.com>
 
 GNU Lesser General Public License.
 See <http://www.gnu.org/licenses/> for details.
 All above must be included in any redistribution
 ****************************************************/

/***********Notice and Trouble shooting***************
 1.Connection and Diagram can be found here: https://www.dfrobot.com/wiki/index.php?title=Capacitive_Soil_Moisture_Sensor_SKU:SEN0193
 2.This code is tested on Arduino Uno.
 3.Sensor is connect to Analog 0 port.
 ****************************************************/
#include <Wire.h>  // Include Wire if you're using I2C

#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
//#define BLYNK_PRINT Serial
//#include <ESP8266_Lib.h>
//#include <BlynkSimpleShieldEsp8266.h>
const int AirValue = 650;   //you need to replace this value with Value_1
const int WaterValue = 350;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/4;   
int soilMoistureValue = 0;


#define PIN_RESET 255  //

#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D
MicroOLED oled(PIN_RESET, DC_JUMPER);  // I2C Example
//char auth[] = "a254c8e7d58041e892b65b391e279927";

void setup() {
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  oled.begin();     // Initialize the OLED

  oled.clear(PAGE); // Clear the display's internal memory

  oled.clear(ALL);  // Clear the library's display buffer

  oled.display();   // Display what's in the buffer (splashscreen)
 // Blynk.begin(auth);
}
void loop() {
soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
oled.clear(PAGE);  // Clear the buffer
oled.setCursor(0, 0);
oled.print("Humidity index: ");          // Print "A0"
oled.setFontType(12);         // 7-segment font
oled.print(soilMoistureValue);  // Print a0 reading
oled.setCursor(0, 20);
if(soilMoistureValue > WaterValue && soilMoistureValue < (WaterValue + intervals))
{
  Serial.println("Very Wet");
  oled.print("Very Wet");          // Print "A0"
}
else if(soilMoistureValue > (WaterValue + intervals) && soilMoistureValue < (WaterValue + 2*intervals))
{
  Serial.println("Wet");
  oled.print("Wet");
}
else if(soilMoistureValue < WaterValue + 3*intervals && soilMoistureValue > (WaterValue + 2*intervals))
{
  Serial.println("Dry");
  oled.print("Dry");
}
else if(soilMoistureValue < WaterValue + 4*intervals && soilMoistureValue > (WaterValue + 3*intervals))
{
  Serial.println("Very Dry");
  oled.print("Very Dry");
}
oled.display(); //  Draw the memory buffer
//Blynk.virtualWrite(0, soilMoistureValue);
//Blynk.run();
delay(100);
}
