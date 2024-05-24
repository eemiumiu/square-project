#include <avr/io.h>
#include <usart.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <led.h>
#include <button.h>

#include <stdbool.h>

extern bool ledState1 = true;
extern bool ledState2 = false;
extern bool ledState3 = false;

ISR(PCINT1_vect)
{
    // if button 1 is pressed
    if(bit_is_clear(PINC,PC1))
    {
        ledState1 = false;
        ledState2 = true;
        ledState3 = false;
    }

    // if button 2 is pressed
    if(bit_is_clear(PINC,PC2))
    {
        ledState1 = false;
        ledState2 = false;
        ledState3 = true;
    }

    // this one i made for myself, button 3
    if(bit_is_clear(PINC,PC3))
    {
        ledState1 = true;
        ledState2 = false;
        ledState3 = false;
    }
}

int main()
{
    enableMultipleLeds(0b00001110);
    lightDownMultipleLeds(0b00001110);

    enableButton(1);
    enableButton(2);

    PCICR |= _BV( PCIE1); /* in Pin Change Interrupt Control Register: indicate
                             * which interrupt(s) you want to activate (PCIE0: port B,
                             * PCIE1: port C, PCIE2: port D) */

    PCMSK1 |= _BV( PC1 ); /* In the corresponding Pin Change Mask Register: indicate
                             * which pin(s) of that port activate the ISR. */
    PCMSK1 |= _BV( PC2 );
    // for myself
    PCMSK1 |= _BV( PC3 );
    

    sei();

    while(1)
    {
        if(ledState1) 
        {
            lightUpMultipleLeds(0b00001100);
            _delay_ms(2000);
            lightDownMultipleLeds(0b00001100);
            _delay_ms(2000);
        }

        if(ledState2) 
        {
            lightUpMultipleLeds(0b00000110);
            _delay_ms(5000);
            lightDownMultipleLeds(0b00000110);
            _delay_ms(5000);
        }

        if(ledState3)
        {
            for(int i = 7; i > 0; i--)
            {
                lightUpLed(3);
                _delay_ms(500);
                lightDownLed(3);
                _delay_ms(500);
                if(!ledState3) break; 
            }
            if(ledState3)
            {
                ledState3 = false;
                ledState1 = true;
            }
            // ledState1 starts running again if the led lights up 7 times successfully
        }
    }
}
