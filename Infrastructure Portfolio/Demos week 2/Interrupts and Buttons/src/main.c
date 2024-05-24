#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PORT    PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN  PINC
#define LED_DDR     DDRB
#define BUTTON_DDR  DDRC
#define BUTTON1     PC1
#define LED1        PB2
#define LED2        PB3
 
/* First, review the code in the main function!
 * This ISR is called when Pin Change Interrupt 1 is triggered (PCINT1_vect)
 * This is the interrupt for PORTC, where the switches/buttons are connected... */
ISR( PCINT1_vect )
{
    // button 1 is pressed (bit is set to 0)?
    if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
    {
        //We wait 1000 microseconds and check again (debounce!)
        _delay_us( 1000 );
        // button 1 is pressed (bit is set to 0)?
        if ( bit_is_clear( BUTTON_PIN, BUTTON1 ))
        {
            if ( bit_is_set( LED_PORT, LED2 ))  // if led2 is off (bit is set to 1)
            {
                LED_PORT &= ~_BV( LED2 );       // turn led2 on (set bit to 0)
            }
            else                                // else
            {
                LED_PORT |= _BV( LED2 );        // turn led2 off (set bit to 1)
            }
        }
    }
}

int main()
{
    LED_DDR |= _BV( LED1 ) | _BV( LED2 );   // we'll use led1 and led2
    LED_PORT |= _BV( LED1 ) | _BV( LED2 );  // turn 2 leds off
    BUTTON_DDR &= ~_BV( BUTTON1 );          // we'll use button1
    BUTTON_PORT |= _BV( BUTTON1 );          // enable pull-up
    
    PCICR |= _BV( PCIE1 );  /* in Pin Change Interrupt Control Register: indicate
                             * which interrupt(s) you want to activate (PCIE0: port B,
                             * PCIE1: port C, PCIE2: port D) */
    
    PCMSK1 |= _BV( PC1 );   /* In the corresponding Pin Change Mask Register: indicate
                                 * which pin(s) of that port activate the ISR. */
    
    sei();  // Set Enable Interrupts --> activate the interrupt system globally.
    
    while ( 1 )
    {
        LED_PORT &= ~_BV( LED1 );   // turn led1 on
        _delay_ms( 100 );
        LED_PORT |= _BV( LED1 );    // turn led1 off
        _delay_ms( 100 );
    }
    return 0;
}

/**

Vector Name   	What is in the Vector?
0	()          -  References the ISR for RESET
1	INT0_vect	-  References the ISR for External Interrupt Request 0
2	INT1_vect	-  References the ISR for External Interrupt Request 1
3	PCINT0_vect	-  References the ISR for interrupts originating from a PORT B pin
4	PCINT1_vect	-  References the ISR for interrupts originating from a PORT C pin
5	PCINT2_vect	-  References the ISR for interrupts originating from a PORT D pin

*/
