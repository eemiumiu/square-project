#include <avr/io.h> // ATmega328 pins, def of the registers (DDRB, PORTB, ...)
#include <util/delay.h>
#include <avr/interrupt.h>

#include <usart.h>  // for Serial Monitor
#include <led.h>
#include <button.h>
#include <display.h>
#include <buzzer.h>

#include <string.h>
#include <stdlib.h>
#include <math.h>
// #include <stdio.h>
// #include <stdbool.h>

/* Byte map for the square states and arrows */
const uint8_t GAME_SEGMENT_MAP[] = {};

// ISR()
// {

// }

void jump()
{

}

void duck()
{

}

void arrow(int segment, int speed)
{

}

typedef struct{
    int life;
    int shield;
    int score;
} PLAYER;

int main()
{
    initUSART();
    initDisplay();

    while(1)
    {
        for ( int i = 0; i < 10000; i ++ )
        {
            displayObject(2553);
        }
    }
}
