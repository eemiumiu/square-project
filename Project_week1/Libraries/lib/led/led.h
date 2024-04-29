/*
Basic functions to control the leds of your multifunctional shield
*/
#include <util/delay.h>
void enableLed ( int lednumber );
void lightUpLed ( int lednumber );
void lightDownLed ( int lednumber );
void enableMultipleLeds (uint8_t leds);
void lightUpMultipleLeds(uint8_t leds);
void lightDownMultipleLeds(uint8_t leds);
void enableAllLeds();
void lightUpAllLeds();
void lightDownAllLeds();
void dimLed(int lednumber, int percentage, int duration);
void fadeInLed(int lednumber, int duration);
void fadeOutLed(int lednumber, int duration);