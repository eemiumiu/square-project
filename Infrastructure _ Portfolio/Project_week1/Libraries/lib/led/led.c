#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>

#define NUMBER_OF_LEDS 4 

void enableLed ( int lednumber ) 
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));
}

void lightUpLed ( int lednumber )    
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber ));  
}

void lightDownLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber ));   
}

//LED functions I created in exercise 1.11
void enableMultipleLeds(uint8_t leds) 
{
    for (int i = 0; i < NUMBER_OF_LEDS; i++) 
    {
        if (leds & (1 << i)) 
        {
            enableLed(i);
        }
    }
}

void lightUpMultipleLeds(uint8_t leds) {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        if (leds & (1 << i)) {
            lightUpLed(i);
        }
    }
}

void lightDownMultipleLeds(uint8_t leds) {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        if (leds & (1 << i)) {
            lightDownLed(i);
        }
    }
}

void enableAllLeds() {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        enableLed(i);
    }
}

void lightUpAllLeds() {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        lightUpLed(i);
    }
}

void lightDownAllLeds() {
    for (int i = 0; i < NUMBER_OF_LEDS; i++) {
        lightDownLed(i);
    }
}

//LED functions I created in exercise 11.2
void dimLed(int lednumber, int percentage, int duration) 
{
    if (lednumber < 0 || lednumber > NUMBER_OF_LEDS - 1) return;
    if (percentage < 0 || percentage > 100) return;

    float delay = (float)20 / (float)100 * percentage;
    int int_delay = delay / 1;

    for (int i = 0; i <= duration; i++)
    {
        lightUpLed(lednumber);
        _delay_ms(int_delay);
        lightDownLed(lednumber);
        _delay_ms(20 - int_delay);
    }

}

void fadeInLed(int lednumber, int duration) 
{
    int duration_dimLed = duration / 1000;
    // for increment = 0 -> 100
    // dimLed(lednumber, increment, duration/smth?)
    for(int i = 0; i <= 100; i++)
    {
        dimLed(lednumber, i, duration_dimLed);
    }
}

void fadeOutLed(int lednumber, int duration) 
{
    int duration_dimLed = duration / 1000;

    for(int i = 100; i >= 0; i--)
    {
        dimLed(lednumber, i, duration_dimLed);
    }
}
