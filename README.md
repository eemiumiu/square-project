The Square Game is a project in C for Arduino Uno board. 
It utilises PlatformIO ,a CLI (or IDE extension), which is used for building the project, doing serial port monitor testing, and installing dependencies like C libraries with definitions for the microcontroller registers. 
The Arduino is equipped with a Multi-function Shield Expansion which utilises a buzzer, 4 buttons, a potentiometer, 4 LEDs and a 4 digits 7 segments LED display.

##### To play the game on the Arduino that's equiped with the Whadda WPSH209 Shield Expantion, open the project in an IDE and use either the PlatformIO CLI or Extention to download the dependencies, build the project and upload it to the Arduino. The PlatformIO extention will read the platformio.ini file when downloading dependencies.

# Square Game
##### Avoid incoming arrows by jumping and ducking !!!
##### Press button 1 to jump
##### Press button 3 to duck
##### To activate a shield, press button 2

##### You have 4 lives so be careful.

```
     ⎻       ♡♡♡♡ ⛨⛨⛨⛨
⃞  _     ⎻   
```

The C program makes use of pointers, efficient memory allocation, communication with hardware components, writing directly to pins, created libraries with my own custom methods. 
