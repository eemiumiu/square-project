#include <avr/io.h> // ATmega328 pins, def of the registers (DDRB, PORTB, ...)
#include <util/delay.h>
#include <avr/interrupt.h>

#include <usart.h>  // Serial Monitor
#include <led.h>
#include <button.h>
#include <buzzer.h>
#include <display.h>    /* added methods for my game as well, like 
                        *  displayObjectToSegment, displayObject, 
                        *  and the arrays: SQUARE_AND_ARROWS_STATE[], 
                        *  LIFE_STATE[] */ 

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

#define DIGIT_1 1000
#define DIGIT_2 100
#define DIGIT_3 10
#define DIGIT_4 1

uint32_t counter1 = 0; // memory?

int* counter_reset = 2000;
int* down_time_cube = 70;

int* generate_arrow = 1000;
int* speed = 150;

bool lock = false;
bool pass1 = false;
bool pass2 = false;
bool pass3 = false;

bool game_play = true; // memory?

const int* empty = 0;
const int* low = 1;
const int* norm = 2;
const int* up = 3;
const int* middle = 4; 
const int* high = 5;

typedef struct{
    int lives;
    int shields;
    int score;
} PLAYER;

typedef struct{
    int display;
    bool ground;
    int state;
} CUBE;

CUBE* cube;

void change_didit_cube(int digit, int state){
    cube -> display = cube -> display % digit + (digit * state);
}

void setGround(bool b){
    cube -> ground = b;
}

bool isCubeOnTheGround(){
    return cube -> ground;
}

change_digit_arrow(int digit, int state){
    //height 3
    //speed = every 250 of counter2
    //2000 = (2000 / 1)*1 + 1*5 
    //2005 = (2005 / 10)*10 + 10*5 
    //2050 = (2050 / 100)*100 + 100*5 
    //special 7000, other

    (cube -> display) = 
    (cube -> display / digit)*digit + (digit * state);
}

void jump()
{    
    change_didit_cube(DIGIT_1, up);
}

void back_down()
{    
    change_didit_cube(DIGIT_1, norm);
    setGround(true);
}

void duck()
{    
    change_didit_cube(DIGIT_1, low);
}

void initTimer()
{
    TCCR0A |= _BV( WGM00 ) | _BV( WGM01 );  
    TCCR0B |= _BV( CS02 ) | _BV( CS00 );    
    TIMSK0 |= _BV( TOIE0 ); 
}

// TOP value (255)
ISR(TIMER0_OVF_vect)
{
    displayObject(cube -> display);
    blankSegment(3); // so last segment is not brighter

    counter1++; 
    if (counter1 > counter_reset){
        counter1 = 0;
    } 

    if (counter1 > down_time_cube) {
        back_down();
        setGround(true);
    }

    if (counter1 / 1000 == 1 && !lock)
    {
        lock = true;

    }

    printf("%d \n", counter1);
}

ISR(PCINT1_vect)
{
    if(bit_is_clear(PINC,PC1))
    {
        if( isCubeOnTheGround() )
        {
            setGround(false);
            jump();
            counter1 = 0;
        }
    }

    if(bit_is_clear(PINC,PC2))
    {
        
    }

    if(bit_is_clear(PINC,PC3))
    {
        if( isCubeOnTheGround() )
        {
            setGround(false);
            duck();
            counter1 = 0;
        }
    }
}

int main()
{
    PLAYER* player =    (PLAYER*)malloc(sizeof(PLAYER));
    cube =              (CUBE*)malloc(sizeof(CUBE));

    initUSART();
    initDisplay();
    initTimer();

    enableAllButtonInterrupts();

    srand(0); // potentiometer

    sei(); 

    cube -> display = 2000;
    cube -> ground = true;
    
    player -> lives = 4;

    change_digit_arrow(DIGIT_4, low);
    _delay_ms(1000);
    change_digit_arrow(DIGIT_3, low);
    _delay_ms(1000);
    change_digit_arrow(DIGIT_2, low);
    _delay_ms(1000);

    // cube -> display = 6666;
    // _delay_ms(1000);
    // cube -> display = 1000;
    
    // while(game_play)
    // {
        // while(!lock);
        // change_digit_arrow(DIGIT_4, high); 

        // change_digit_arrow(DIGIT_3, high); 

        // change_digit_arrow(DIGIT_2, high); 

        // change_digit_arrow(DIGIT_1, high); 
    
    // };
    
    free(cube);
    free(player);

    return 0;
}
