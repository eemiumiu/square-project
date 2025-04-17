#include <avr/io.h> // ATmega328 pins, defs of the registers (DDRB, PORTB, ...)
#include <util/delay.h>
#include <avr/interrupt.h>

#include <usart.h>  // Serial Monitor
#include <led.h>
#include <button.h>
#include <buzzer.h>
#include <display.h>    /* display.h contains custom methods I created: 
                        *  displayObjectToSegment(), displayObject(), 
                        *  and the byte_maps: SQUARE_AND_ARROWS_STATE[], 
                        *  LIFE_STATE[] */ 

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>

// #define DEBUG

#define DIGIT_1 1000 
#define DIGIT_2 100
#define DIGIT_3 10
#define DIGIT_4 1

const int empty = 0;
const int low = 1;
const int norm = 2;
const int up = 3;

uint16_t counter1 = 0; 
uint16_t potentiometer_seed = 0;

bool game_play = true; 
bool opening_tune = true;
bool ending_tune = false;
bool hit1 = true;
bool hit2 = true;
bool hit3 = true;
bool hit4 = true;

int lives = 4;
int shields = 4;
int used_shields = 0;

typedef struct{
    int pos[3];
    int digit; 
    uint16_t generate_time;
    int speed;
} ARROW;

typedef struct{
    int score;
} PLAYER;

typedef struct {
    int display;
    bool ground;
    int counter_reset;
    int down_time_cube;
} CUBE;

CUBE* cube;

void change_digit_1_cube(const int *state){
    cube -> display = cube -> display % DIGIT_1 + (DIGIT_1 * *state);
}

void setGround(bool b){
    cube -> ground = b;
}

bool isCubeOnTheGround(){
    return cube -> ground;
}

void update_state(int digit, int state, ARROW **arrow, PLAYER **player)
{
    (cube -> display) = (cube -> display / digit) * digit + (digit * state);

    if(digit == DIGIT_1)
    {
        #ifdef DEBUG
        printf("Display: %d \n", cube -> display);
        printf("Arrow: %d \n", (*arrow) -> pos[(*arrow) -> digit]);
        #endif

        if ((cube -> display / 1000 == 1 && (*arrow) -> pos[(*arrow) -> digit] == 1 )
            || (cube -> display / 1000 == 2 && ((*arrow) -> pos[(*arrow) -> digit] == 1 || (*arrow) -> pos[(*arrow) -> digit] == 4))
            || (cube -> display / 1000 == 3 && ((*arrow) -> pos[(*arrow) -> digit] == 4 || (*arrow) -> pos[(*arrow) -> digit] == 5))) 
        {
            lives --; 
            // printf("Dead !\n");
        } else {
            (*player) -> score ++; 
            // printf("Good job !\n");
        }

        if(isCubeOnTheGround() && (*arrow) -> pos[(*arrow) -> digit] == 5) 
            specialCase(1);
        if((cube -> display) / 1000 == 3 && (*arrow) -> pos[(*arrow) -> digit] == 1 ) 
            specialCase(2);
        if((cube -> display) / 1000 == 1 && (*arrow) -> pos[(*arrow) -> digit] == 5 ) 
            specialCase(3);
        if((cube -> display) / 1000 == 1 && (*arrow) -> pos[(*arrow) -> digit] == 4 ) 
            specialCase(4);
        
        _delay_ms((*arrow) -> speed);

        specialCase(0); //specialCase() - so arrow can be displayed on the same digit as the cube
    }
}

void jump()
{    
    change_digit_1_cube(&up);
}

void back_down()
{    
    change_digit_1_cube(&norm);
    setGround(true);
}

void duck()
{    
    change_digit_1_cube(&low);
}

void generate_arrow(ARROW *arrow, PLAYER *player)
{
    arrow -> digit = rand() % 3;

    update_state(DIGIT_4, arrow -> pos[arrow -> digit], &arrow, &player);
    _delay_ms(arrow -> speed);
    update_state(DIGIT_3, arrow -> pos[arrow -> digit], &arrow, &player);
    _delay_ms(arrow -> speed);
    update_state(DIGIT_2, arrow -> pos[arrow -> digit], &arrow, &player);
    _delay_ms(arrow -> speed);
    
    update_state(DIGIT_1, empty, &arrow, &player);
}

void initADC()
{
    ADMUX |= ( 1 << REFS0 ) ;   //Set up of reference voltage (5V as reference)
    ADMUX &= ~(1 << MUX3  ) & ~(1 << MUX2  ) & ~(1 << MUX1 ) & ~(1 << MUX0 );
                                //Set MUX(0-3) to zero to read analog input from PC0
                                //Default will still be 0000 
    ADCSRA |= ( 1 << ADEN ); //Enable the ADC
}

void initTimer()
{
    TCCR0A |= _BV( WGM00 ) | _BV( WGM01 );  
    TCCR0B |= _BV( CS02 ) | _BV( CS00 );    
    TIMSK0 |= _BV( TOIE0 ); 
}

ISR(TIMER0_OVF_vect)
{
    displayObject(cube -> display);
    blankSegment(3); // so the last segment is not brighter

    counter1++; 
    if (counter1 > cube -> counter_reset) 
    {
        counter1 = 0;
    } 

    if (counter1 > cube -> down_time_cube) 
    {
        back_down();
        setGround(true);
    }

    switch(lives) {
    case 3: lightDownLed(3); break;
    case 2: lightDownLed(2); break;
    case 1: lightDownLed(1); break;
    case 0: lightDownLed(0); break;
    }

    // Potentiometer seed
    ADCSRA |= ( 1 << ADSC );  
    loop_until_bit_is_clear( ADCSRA, ADSC );    
    potentiometer_seed = ADC; 
    ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );

    gameSoundtrack();
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
        shields--;
        if(used_shields < 4) 
        { 
            lives++; used_shields++; displayShields(shields); 
        }
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
    cube =              (CUBE*)malloc(sizeof(CUBE));

    initUSART();
    initADC();
    initDisplay();
    initTimer();
    enableBuzzer();

    enableAllButtonInterrupts();
    enableAllLeds();

    sei(); 

    cube -> display = 2000;
    cube -> ground = true;
    cube -> counter_reset = 3000;
    cube -> down_time_cube = 70;
    
    player -> score = 0;

    *arrow = (ARROW){{1, 4, 5}, 0};
    arrow -> generate_time = 1200;
    arrow -> speed = 350;

    _delay_ms(500);

    printf("\n WELCOME TO SQUARE GAME \n");
    printf(" Avoid incoming arrows by jumping and ducking !!!\n");
    printf("\n RULES: \n");
    printf(" 1. Press button 1 to jump \n");
    printf(" 2. Press button 3 to duck \n");
    printf(" 3. To activate a shield, press button 2 \n");

    srand(potentiometer_seed); 
    #ifdef DEBUG
    printf("Potentiometer: %d \n", potentiometer_seed);
    #endif

    while(game_play)
    {

        _delay_ms(arrow -> generate_time);

        generate_arrow(arrow, player);

        displayShields(shields);

        if(lives == 0)
        {
            ending_tune = true;
            printf("\n END GAME \n\n Score obtained: %d \n Shields used:   %d \n ", player -> score, used_shields);
            game_play = false;
        }
         
    };
    
    free(player);
    free(arrow);
    free(cube);

    return 0;
}

void gameSoundtrack()
{
    if(opening_tune)
    {
        for(int i=0; i<2; i++)
        {
            playTone( 987.770, 50 ); 
            _delay_ms(50);
            playTone( 1046.500, 50 ); 
            _delay_ms(50);
        }       
        playTone( 1046.500, 100 );  
        _delay_ms(100);
        playTone( 987.770, 150 ); 
        _delay_ms(100);
        playTone( 783.990, 100 ); 
        _delay_ms(100);
        playTone( 987.770, 100 );
        _delay_ms(100);
        playTone( 1046.500, 100 );  

        opening_tune = false;
    }

    if(lives<4 && hit1)
    {
        playTone( 987.770, 100 );
        hit1 = false;
    } 

    if(lives<3 && hit2)
    {
        playTone( 987.770, 100 );
        hit2 = false;
    } 

    if(lives<2 && hit3)
    {
        playTone( 987.770, 100 );
        hit3 = false;
    } 

    if(lives<1 && hit4)
    {
        playTone( 987.770, 100 );
        hit4 = false;
    } 

    if(ending_tune)
    {
        playTone( 783.990, 100 ); 
        _delay_ms(200);
        playTone( 987.770, 100 );
        _delay_ms(200);
        playTone( 1046.500, 100 ); 
        _delay_ms(200);    

        ending_tune = false;
    }
}