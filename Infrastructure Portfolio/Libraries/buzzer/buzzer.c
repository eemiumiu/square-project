#include <avr/io.h>
#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

void enableBuzzer() // enables the buzzer
{
       DDRD |= ( 1 << PD3 ); 
}

void playTone( float frequency, uint32_t duration ) // plays a tone
{
    uint32_t periodInMicro = ( uint32_t ) ( 1000000 / frequency ); //Calculate the period in microsecs from the freq
    uint32_t durationInMicro = duration * 1000; //We express duration in microsecs
    
    for ( uint32_t time = 0; time < durationInMicro; time += periodInMicro )
    {
        PORTD &= ~( 1 << PD3 ); //turn the buzzer on
        _delay_us( periodInMicro / 2 ); //Wait for the half of the period
        PORTD |= ( 1 << PD3 ); //Turn the buzzer off
        _delay_us( periodInMicro / 2 ); //Wait again for half of the period
    }
}

void disableBuzzer() // disables the buzzer
{
    DDRD &= ~(1 << PD3); // PD3 as input
    PORTD &= ~(1 << PD3); // pin is low
    PORTD |= ( 1 << PD3 ); //Turn the buzzer off
}

// void playMusic(**notes)
// {
    
// }