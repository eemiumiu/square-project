#include <avr/io.h>
#include <usart.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <led.h>
#include <button.h>

#include <stdbool.h>

extern bool pressed1 = false;
extern bool pressed2 = false;
extern bool pressed3 = false;

extern int choice = 0;
extern int correct = 0;

extern int light_combination_generated[10] = {};

ISR(PCINT1_vect)
{
    // if button 1 is pressed
    if(bit_is_clear(PINC,PC1))
    {
        pressed1 = true;
    }
    // if button 2 is pressed
    if(bit_is_clear(PINC,PC2))
    {
        pressed2 = true;
    }
    // if button 3 is pressed
    if(bit_is_clear(PINC,PC3))
    {
        pressed3 = true;
    }
}

int main()
{
    initUSART();

    srand(0); // seed for randomiser

    enableAllButtonInterrupts();
    enableAllLeds();
    lightDownAllLeds();

    sei();

    printf("\nStart the game !!\npress button 1 →\n");
    while(!pressed1)
    {
        lightUpLed(3);
        _delay_ms(200);
        lightDownLed(3);
        _delay_ms(200);
    }

    //for() to initialize the 10 blink combinations into an array

    for(int i = 0; i < 10; i++)
    {
        // fot(lenght)
        // {
        //     printf("Simon says...")
        // }
        if(choice != correct) i = 10;
    }

    //end game

}
