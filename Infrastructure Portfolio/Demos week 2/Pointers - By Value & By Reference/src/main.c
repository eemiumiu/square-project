#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

void enableLed( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));
}

// void writeMask( uint8_t registerr, uint8_t bitmask )
// {
//     registerr = bitmask;
// }

void writeMask( volatile uint8_t* registerr, uint8_t bitmask )
{
    *registerr = bitmask;
}

int main()
{
    int lednumber = 0;

    uint8_t bitmask = ( 1 << ( PB2 + lednumber ));  //prepare a bitmask
    writeMask( &DDRB, bitmask );                     // Write the bitmask to the register

    return 0;
}
