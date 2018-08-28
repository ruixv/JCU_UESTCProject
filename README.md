# JCU_UESTCProject
 + 电子科技大学-JCU交流项目-自动灌溉系统
 + 时间: Jul 2018

## Intruduction
  + Our design consists of three main parts: software design， hardware design.， and the appearence of our design. 
  In hardware design, Since we are using modules rather than individual components, and the impedance matching between these modules is quite ideal, our hardware design is mainly concerned with hardware connections.
  +The software design mainly includes the code design of Arduino and the APP interface design of Android Blynk. Among them, the code design is shown in this logic diagram.
    + We first read the current air temperature value through the SHT30 air temperature and humidity sensor. When the temperature is greater than 30, the temperature is too high at this time, and if the irrigation continues, it is not conducive to the health of the plant. Therefore, once the air temperature value is greater than 30, we cannot continue to irrigate. At this time, if the soil moisture is greater than 500, this means that the soil is dry and needs to be watered, but because the temperature is too high, we can't water it, so the LED is lit up to warn to manually water the water.
    + We can also choose the irrigation mode through Blynk. 
      + In the automatic mode, the soil drying value is greater than 500, we will open the switch for watering, and when the humidity value is less than 400, the switch will stop watering. The reason for setting two thresholds is to prevent the occurrence of shocks.
      + In manual mode, we can control the irrigation system by simply controlling the switches on Blynk.
  + On our Blynk App, we set up two switches to control WeMos, four display boxes to display the value of the sensors, and setting a Graph to make the change of different variables over time clearer.
  + This is our hardware connection diagram with a total of one controller, one relay, one LED, one OLED and two sensors.
  Among them, SHT30 and OLED use I2C for data transmission, so their wiring is the same; the other three have a data port for data transmission or control.
  + With the previous hardware connection diagram, we can put them in a tray. 

## How to Start
  +You need some hardware as followes:
    + WeMos D1 mini Board
    + Soil Moisture Sensor
    + SHT30 Air temperature and moisture snesor
    + OLED with I2C
    + 12V Relay
    + Blynk Android/IOS APP
  + 
  
