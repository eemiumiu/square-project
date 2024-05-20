#include <Arduino.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

#include <usart.h>
#include <led.h>

// !!!!
// the lib_extra_dirs in platformio.ini, may give an error because 
// on Mac and on Windows, path sidebars are diffrent ( / and \ ).

#define NUMBER_OF_LEDS 4

int main()
{
  // seed for random
  srand(0);

  // led output
    DDRB |= (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);

    while (1) 
    {
        // LED to toggle
        int ledToToggle = getRandomInRange(0, NUMBER_OF_LEDS - 1);
        lightUpLed(ledToToggle);
        int delayTime = getRandomInRange(100, 1000);
        _delay_ms(delayTime);
        lightDownLed(ledToToggle);
        int delayTime2 = getRandomInRange(100, 1000);
        _delay_ms(delayTime2);
    }
}

int getRandomInRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

