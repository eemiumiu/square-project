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

const int* empty = 0;
const int* down = 1;
const int* norm = 2;
const int* up = 3;
const int* middle = 4; 
const int* high = 5;

const int* digit_1 = 1000; 
const int* digit_2 = 100; 
const int* digit_3 = 10; 
const int* digit_4 = 1; 

uint32_t* counter = 0;

bool game_play = true;

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

CUBE** cube;

void change_state(int digit, int state)
{
    (*cube) -> display = (*cube) -> display % digit + (digit * state);
}

void set_ground(bool b)
{
    (*cube) -> ground = b;
}

void jump()
{    
    change_state(digit_1, up);
    set_ground(false);
}

void back_down()
{    
    change_state(digit_1, norm);
    set_ground(true);
}

void duck()
{    
    change_state(digit_1, down);
    set_ground(true);
}

void random_arrow(int segment, int speed)
{

}

void initTimer()
{
    TCCR0A |= _BV( WGM00 ) | _BV( WGM01 );  
    TCCR0B |= _BV( CS02 ) | _BV( CS00 );    
    TIMSK0 |= _BV( TOIE0 ); 
    TIMSK0 |= _BV( OCIE0A );

    sei();
}

ISR(PCINT1_vect)
{
    // left button
    if(bit_is_clear(PINC,PC1))
    {
        if( (*cube) -> ground )
        {
            set_ground(false);
            jump();
            counter = 0;
        }
    }

    if(bit_is_clear(PINC,PC2))
    {
        
    }

    if(bit_is_clear(PINC,PC3))
    {
        if( (*cube) -> ground )
        {
            set_ground(false);
            duck();
            counter = 0;
        }
    }
}

// OCRA register
ISR( TIMER0_COMPA_vect )
{
    // displayObject(cube -> display);
    counter++;
    if (counter > 2000) counter = 0;

    if(counter > 500) 
    {
        back_down();
        (*cube) -> ground = true;
    }
}

// TOP value (255)
ISR(TIMER0_OVF_vect)
{
    displayObject((*cube) -> display);
    blankSegment(3); // so segment 4 is not brighter
    counter++;
}

int main()
{
    initUSART();
    initDisplay();

    enableAllButtonInterrupts();

    initTimer();

    srand(0); // potentiometer

    PLAYER* player =    (PLAYER*)malloc(sizeof(PLAYER));
    ARROW* arrow =      (ARROW*)malloc(sizeof(ARROW));

    cube =              (CUBE*)malloc(sizeof(CUBE));

    (*cube) -> display = 2000;
    (*cube) -> ground = true;
    
    while(game_play)
    {
        int hi;
        // OCR0A = 001;
        // if (counter > 2000) counter = 0;
    }
    
    free(cube);
    free(player);
    free(arrow);

    return 0;
}
