#include <avr/io.h>
#include <usart.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <led.h>
#include <button.h>

#include <stdbool.h>

extern bool led0state = false;
extern bool led1state = false;
extern bool led2state = false;

ISR(PCINT1_vect)
{
    // if button 1 is pressed
    if(bit_is_clear(PINC,PC1))
    {
        if(!led0state)
        {
            printf("button pressed\n");
            led0state = true;
        } else {
            printf("button pressed\n");
            led0state = false;
        }
    }

    if(bit_is_clear(PINC,PC2))
    {
        if(!led1state)
        {
            printf("button pressed\n");
            led1state = true;
        } else {
            printf("button pressed\n");
            led1state = false;
        }
    }
    if(bit_is_clear(PINC,PC3))
    { 
        if(!led2state)
        {
            printf("button pressed\n");
            led2state = true;
        } else {
            printf("button pressed\n");
            led2state = false;
        }
    } 
}

int main()
{
    initUSART();

    // enabling the first 3 leds 
    enableMultipleLeds(0b00001111);
    lightDownMultipleLeds(0b00001111);

    enableButton(1);
    enableButton(2);
    enableButton(3);

    // activate interrupt PCIE1: port C
    PCICR |= _BV( PCIE1);

    // the pins that activate the ISR
    PCMSK1 |= _BV( PC1 );
    PCMSK1 |= _BV( PC2 );
    PCMSK1 |= _BV( PC3 );

    sei();

    while(1)
    {
        // with 1 led
        if(led0state && !led1state && !led2state)
        {
            lightUpLed(0);
            _delay_ms(100);
            lightDownLed(0);
            _delay_ms(100);
        }
        if(!led0state && led1state && !led2state)
        {
            lightUpLed(1);
            _delay_ms(100);
            lightDownLed(1);
            _delay_ms(100);
        }
        if(!led0state && !led1state && led2state)
        {
            lightUpLed(2);
            _delay_ms(100);
            lightDownLed(2);
            _delay_ms(100);
        }

        // with 2 leds
        if(led0state && led1state && !led2state) 
        {
            lightUpMultipleLeds(0b00000011);
            _delay_ms(100);
            lightDownMultipleLeds(0b00000011);
            _delay_ms(100);
        }  
        if(!led0state && led1state && led2state) 
        {
            lightUpMultipleLeds(0b00000110);
            _delay_ms(100);
            lightDownMultipleLeds(0b00000110);
            _delay_ms(100);
        }
        if(led0state && !led1state && led2state) 
        {
            lightUpMultipleLeds(0b00000101);
            _delay_ms(100);
            lightDownMultipleLeds(0b00000101);
            _delay_ms(100);
        }

        // with 3 leds          
        if(led0state && led1state && led2state) 
        {
            lightUpMultipleLeds(0b00000111);
            _delay_ms(100);
            lightDownMultipleLeds(0b00000111);
            _delay_ms(100);
        }     

        _delay_ms(100);
    }
}
