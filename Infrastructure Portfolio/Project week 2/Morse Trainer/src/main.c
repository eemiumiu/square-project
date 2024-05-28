#include <avr/io.h>
#include <usart.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <led.h>
#include <button.h>

#include <stdbool.h>

extern char choice1 = '-';
extern char choice2 = '-';
extern char choice3 = '-';

extern char letter_lit = '-';
extern int correct = 0;
extern int score = 0;

extern bool pressed = false;
extern int selected = 0;
char random_Char();

char answers[] = {'A','Z','R','U','F','K','C','9','7','H'};
#define OPTIONS_ARRAY_LENGTH 10

// #define DEBUG

ISR(PCINT1_vect)
{
    // if button 1 is pressed
    if(bit_is_clear(PINC,PC1))
    {
        selected = 1;
        pressed = true;
    }
    // if button 2 is pressed
    if(bit_is_clear(PINC,PC2))
    {
        selected = 2;
        pressed = true;
    }
    // if button 3 is pressed
    if(bit_is_clear(PINC,PC3))
    {
        selected = 3;
        pressed = true;
    }
}

int main()
{
    initUSART();

    srand(0); // seed for randomiser

    enableAllButtonInterrupts();

    sei();

    printf("\nWelcome to Morse Trainer !!!!\n");

    enableAllLeds();
    _delay_ms(600);
    lightDownLed(3);
    _delay_ms(600);
    lightDownLed(2);
    _delay_ms(600);
    lightDownLed(1);
    _delay_ms(600);
    lightDownLed(0);

    for(int i = 0; i < 5; i++)
    {
        _delay_ms(1500);
        printf("Guess the morse character: \n\n");
        _delay_ms(100);

        letter_lit = answers[randomChoice()];

        executeLightMethod(letter_lit);

        assignPicks();
        _delay_ms(500);

        printf("button 1 → %c , button 2 → %c , button 3 → %c \n", choice1, choice2, choice3);
        
        pressed = false;
        while(!pressed)
        {
            _delay_ms(100);
        }

        if(selected == correct) score++;

        printf("Score: %d Correct Answer: %c \n\n",score ,letter_lit);

        pressed = false;

    }

    printf("END GAME");

    lightdance();
}

void assignPicks()
{
    int num = (rand() % 3) + 1;

    switch(num)
    {
        case 1: {
            choice1 = letter_lit;
            choice2 = random_Char();
            choice3 = random_Char();
            correct = 1;
            break;
        }
        case 2: {
            choice1 = random_Char();
            choice2 = letter_lit;
            choice3 = random_Char();
            correct = 2;
            break;
        }
        case 3: {
            choice1 = random_Char();
            choice2 = random_Char();
            choice3 = letter_lit;
            correct = 3;
            break;
        }
    }
}

char random_Char()
{
    int num;

    while (1) {
        num = rand() % 91;

        if ((num >= 48 && num <= 57) || (num >= 65 && num <= 90)) {
            #ifdef DEBUG
            printf("random char: %c \n", (char)num);
            #endif

            return (char)num;
        }
    }
}

int randomChoice()
{
    /* LETTERS_ARRAY_LENGTH = 10 */
    /* random int between 0 and 9 */
    int r = rand() % 10;

    #ifdef DEBUG
    printf("random choice: %d \n", r);
    #endif

    return r;
}

void letterA(char check)
{
    if(check != 'A') return;
    shortLight();
    longLight();
}

void letterZ(char check)
{
    if(check != 'Z') return;
    longLight();
    longLight();
    shortLight();
    shortLight();
}

void letterR(char check)
{
    if(check != 'R') return;
    shortLight();
    longLight();
    shortLight();
}

void letterF(char check)
{
    if(check != 'F') return;
    shortLight();
    shortLight();
    longLight();
    shortLight();
}

void letterU(char check)
{
    if(check != 'U') return;
    shortLight();
    shortLight();
    longLight();
}

void letterK(char check)
{
    if(check != 'K') return;
    longLight();
    shortLight();
    longLight();
}

void letterC(char check)
{
    if(check != 'C') return;
    longLight();
    shortLight();
    longLight();
    shortLight();
}

void number9(char check)
{
    if(check != '9') return;
    longLight();
    longLight();
    longLight();
    longLight();
    shortLight();
}

void number7(char check)
{
    if(check != '7') return;
    longLight();
    longLight();
    shortLight();
    shortLight();
    shortLight();
}

void letterH(char check)
{
    if(check != 'H') return;
    shortLight();
    shortLight();
    shortLight();
    shortLight();
}

void shortLight()
{
    lightUpAllLeds();
    _delay_ms(400);
    lightDownAllLeds();
    _delay_ms(400);
}

void longLight()
{
    lightUpAllLeds();
    _delay_ms(1700);
    lightDownAllLeds();
    _delay_ms(400);
}

int getRandomInRange(int min, int max) 
{
    return min + rand() % (max - min + 1);
}

void lightdance()
{
    lightUpAllLeds();
    while (1) 
    {
        int ledToToggle = getRandomInRange(0, 4 - 1);
        lightUpLed(ledToToggle);
        _delay_ms(50);
        ledToToggle = getRandomInRange(0, 4 - 1);
        lightDownLed(ledToToggle);
        _delay_ms(50);
    }
}

void executeLightMethod(char check)
{
    letterA(check);
    letterZ(check);
    letterR(check);
    letterF(check);
    letterU(check);
    letterK(check);
    letterC(check);
    number9(check);
    number7(check);
}
