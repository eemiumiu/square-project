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

const int empty = 0;
const int low = 1;
const int norm = 2;
const int up = 3;

uint32_t counter1 = 0; 

bool game_play = true; 

typedef struct{
    int* pos[3];
    int digit; 
    //
    int generate_arrow;
    int speed;
} ARROW;

typedef struct{
    int lives;
    int shields;
    int score;
} PLAYER;

typedef struct {
    int display;
    bool ground;
    int state;
    //
    int counter_reset;
    int down_time_cube;
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

    void change_digit_arrow(int digit, int state, ARROW **arrow)
    {
        (cube -> display) = 
        (cube -> display / digit)*digit + (digit * state);

        if(digit == DIGIT_1)
        {
            // printf("%d \n", (*arrow) -> pos[(*arrow) -> digit]);
            if(isCubeOnTheGround() && (*arrow) -> pos[(*arrow) -> digit] == 5) specialCase(1);
            if((cube -> display) / 1000 == 3 && (*arrow) -> pos[(*arrow) -> digit] ==1 ) specialCase(2);
            if((cube -> display) / 1000 == 1 && (*arrow) -> pos[(*arrow) -> digit] ==5 ) specialCase(3);
            if((cube -> display) / 1000 == 1 && (*arrow) -> pos[(*arrow) -> digit] ==4 ) specialCase(4);
            _delay_ms((*arrow) -> speed);
            specialCase(0);
        }
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

    void getRandomArrow(ARROW *arrow)
    {
        arrow -> digit = rand() % 3;

        change_digit_arrow(DIGIT_4, arrow -> pos[arrow -> digit], &arrow);
        _delay_ms(arrow -> speed);
        change_digit_arrow(DIGIT_3, arrow -> pos[arrow -> digit], &arrow);
        _delay_ms(arrow -> speed);
        change_digit_arrow(DIGIT_2, arrow -> pos[arrow -> digit], &arrow);
        _delay_ms(arrow -> speed);
        change_digit_arrow(DIGIT_1, empty, &arrow);
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
    if (counter1 > cube -> counter_reset){
        counter1 = 0;
    } 

    if (counter1 > cube -> down_time_cube) {
        back_down();
        setGround(true);
    }
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
    ARROW* arrow =      (ARROW*)malloc(sizeof(ARROW));
    *arrow =            (ARROW) {{1, 4, 5}, 0};
    cube =              (CUBE*)malloc(sizeof(CUBE));

    initUSART();
    initDisplay();
    initTimer();

    enableAllButtonInterrupts();

    srand(0); // potentiometer

    sei(); 

    cube -> display = 2000;
    cube -> ground = true;
    cube -> counter_reset = 2000;
    cube -> down_time_cube = 70;
    
    player -> lives = 4;
    player -> shields = 4;
    player -> score = 0;

    arrow -> generate_arrow = 800;
    arrow -> speed = 400;

    _delay_ms(1000);

    while(game_play)
    {
        _delay_ms(arrow -> generate_arrow);
        getRandomArrow(arrow);

        printf("Score: %d \n");
    
    };
    
    free(player);
    free(arrow);
    free(cube);

    return 0;
}
