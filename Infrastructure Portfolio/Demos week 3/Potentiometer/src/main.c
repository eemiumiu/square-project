#include <avr/io.h> // ATmega328 pins, def of the registers (DDRB, PORTB, ...)
#include <usart.h>  // for Serial Monitor
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void initADC()
{
    ADMUX |= ( 1 << REFS0 ) ;   //Set up of reference voltage. We choose 5V as reference.
    ADMUX &= ~(1 << MUX3  ) & ~(1 << MUX2  ) & ~(1 << MUX1 ) & ~(1 << MUX0 );
                                //Set MUX0-3 to zero to read analog input from PC0
                                //Default is 0000 so this setting is not really necessary
                                //Determine a sample rate by setting a division factor. 
                                //Used division factor: 128
    ADCSRA |= ( 1 << ADEN ); //Enable the ADC
}

int main()
{
    initUSART();
    initADC();
    while ( 1 )
    {
        ADCSRA |= ( 1 << ADSC );    //Start the analog --> digital conversion
        loop_until_bit_is_clear( ADCSRA, ADSC );    //Wait until the conversion is completed
        uint16_t value = ADC;  ADCSRA |= ( 1 << ADPS2 ) | ( 1 << ADPS1 ) | ( 1 << ADPS0 );  
     //Read the result
        printf( "Value: %d\n", value );
        _delay_ms(1000);
    }
    return 0;
}