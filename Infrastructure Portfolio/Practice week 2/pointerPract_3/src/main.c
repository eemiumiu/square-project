#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

#include <string.h>

#define MAX 7

void printFirstLetters(char (*arrayN)[10])
{
    initUSART();

    for(int i = 0; i < MAX; i++)
    {
        printf("%c", *(arrayN[i]));
    }
}

int main()
{
    initUSART();

    char names[MAX][10] = {"Moe", "Larry", "Curly", "Yaya", "Jen", "Sarah", "Kyle"};

    printFirstLetters(names);

    return 0;
}
