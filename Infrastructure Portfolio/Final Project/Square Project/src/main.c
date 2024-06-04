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
#include <stdio.h>
#include <stdbool.h>

/* Byte map for the square states and arrows - in the display.c */

typedef struct{
    int lives;
    int shields;
    int score;
} PLAYER;

typedef struct{
    int number;
    int height;
    int speed;
} ARROW;

typedef struct{
    int display;
    bool ground;
} CUBE;

CUBE* cube;


void jump()
{
    cube -> display = cube -> display % 1000 + (1000 * 3);
    cube -> ground = false;
}

void back_down()
{
    cube -> display = cube -> display % 1000 + (1000 * 2);
    cube -> ground = true;
}

void duck()
{
    // return value*1000;

}

void random_arrow(int segment, int speed)
{

}

ISR(PCINT1_vect)
{
    // left button
    if(bit_is_clear(PINC,PC1))
    {
        printf("button 1 pressed\n");
        if( cube -> ground )
        {
            jump();
            
            // Use a timer instead of _delay_ms
            _delay_ms(1000);
            back_down();
        }
    }

    if(bit_is_clear(PINC,PC2))
    {
        
    }

    if(bit_is_clear(PINC,PC3))
    {
        
    }
}

int main()
{
    initUSART();
    initDisplay();
    enableAllButtonInterrupts();

    srand(0); // potentiometer

    PLAYER* player =    (PLAYER*)malloc(sizeof(PLAYER));
    ARROW* arrow =      (ARROW*)malloc(sizeof(ARROW));

    cube =              (CUBE*)malloc(sizeof(CUBE));

    cube -> display = 2000;
    
    // alloc_external_var(); maybe idk

    sei();

    while(1)
    {
        displayObject(cube -> display);
    }
    
    free(cube);
    free(player);
    free(arrow);

    return 0;
}
