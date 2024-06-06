#include <avr/io.h>
#include <usart.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>

#include <led.h>
#include <button.h>

#define DELAY 500
#define MAX_LEVEL 10

volatile bool button_pushed = false;
volatile bool pressed1 = false;
volatile bool pressed2 = false;
volatile bool pressed3 = false;
uint8_t light_combination_generated[MAX_LEVEL];
uint8_t light_combination_entered[MAX_LEVEL];
uint8_t current_level = 0;
int counter = 0;

ISR(PCINT1_vect)
{
    _delay_ms(50); 
    // If button 1 is pressed
    if (bit_is_clear(PINC, PC1))
    {
        pressed1 = true;
        button_pushed = true;
    }
    // If button 2 is pressed
    if (bit_is_clear(PINC, PC2))
    {
        pressed2 = true;
        button_pushed = true;
    }
    // If button 3 is pressed
    if (bit_is_clear(PINC, PC3))
    {
        pressed3 = true;
        button_pushed = true;
    }
}

void generatePuzzle(uint8_t *array, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        array[i] = rand() % 3; 
    }
}

void printPuzzle(uint8_t *array, uint8_t length)
{
    printf("[");
    for (uint8_t i = 0; i < length; i++)
    {
        printf("%d", array[i] + 1);
        if (i < length - 1) printf(" ");
    }
    printf("]\n");
}

void playPuzzle(uint8_t *array, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        switch (array[i])
        {
            case 0:
                lightUpLed(1);
                _delay_ms(DELAY);
                lightDownLed(1);
                break;
            case 1:
                lightUpLed(2);
                _delay_ms(DELAY);
                lightDownLed(2);
                break;
            case 2:
                lightUpLed(3);
                _delay_ms(DELAY);
                lightDownLed(3);
                break;
        }
        _delay_ms(DELAY);
    }
}

bool readInput(uint8_t *array, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        while (true)
        {
            if (pressed1)
            {
                pressed1 = false;
                light_combination_entered[i] = 0;
                if (light_combination_entered[i] != array[i])
                {
                    printf("You pressed button 1, wrong!\n");
                    return false;
                }
                printf("You pressed button 1, correct!\n");
                break;
            }
            if (pressed2)
            {
                pressed2 = false;
                light_combination_entered[i] = 1;
                if (light_combination_entered[i] != array[i])
                {
                    printf("You pressed button 2, wrong!\n");
                    return false;
                }
                printf("You pressed button 2, correct!\n");
                break;
            }
            if (pressed3)
            {
                pressed3 = false;
                light_combination_entered[i] = 2;
                if (light_combination_entered[i] != array[i])
                {
                    printf("You pressed button 3, wrong!\n");
                    return false;
                }
                printf("You pressed button 3, correct!\n");
                break;
            }
        }
    }
    return true;
}

int main()
{
    initUSART();

    enableAllButtonInterrupts();
    enableAllLeds();
    lightDownAllLeds();
    sei();

    printf("Press button 1 to start the game\n");
    while (!button_pushed)
    {
        lightUpLed(4);
        _delay_ms(100);
        lightDownLed(4);
        _delay_ms(100);
        counter++;
    }

    pressed1 = false;
    pressed2 = false;
    pressed3 = false;
    button_pushed = false;

    srand(counter); // Seed for randomizer

    generatePuzzle(light_combination_generated, MAX_LEVEL);

    while (current_level < MAX_LEVEL)
    {
        current_level++;
        printf("Level %d: ", current_level);
        printPuzzle(light_combination_generated, current_level);

        playPuzzle(light_combination_generated, current_level);

        printf("Enter the sequence...\n");
        _delay_ms(1000); 

        if (readInput(light_combination_generated, current_level))
        {
            printf("Correct, we go to level %d\n", current_level);
            for (int i = 0; i < 3; i++)
            {
                lightUpLed(4);
                _delay_ms(200);
                lightDownLed(4);
                _delay_ms(200);
            }
        }
        else
        {
            printf("Wrong, the correct pattern was: ");
            printPuzzle(light_combination_generated, current_level);
            lightUpAllLeds();
            _delay_ms(1000);
            lightDownAllLeds();
            current_level = 0;
            printf("Press button 1 to start the game\n");
            button_pushed = false;
            while (!button_pushed)
            {
                lightUpLed(4);
                _delay_ms(100);
                lightDownLed(4);
                _delay_ms(100);
            }

            pressed1 = false;
            pressed2 = false;
            pressed3 = false;
            button_pushed = false;

            srand(counter); // Seed for randomizer again
            generatePuzzle(light_combination_generated, MAX_LEVEL); 
        }
    }

    printf("Congratulations, you are the Simon Master!\n");
    return 0;
}
