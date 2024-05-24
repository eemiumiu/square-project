#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

//Define 2 macros
#define MAX 5
#define SEVENFOLD(num) (num + 7)

void print_Array(int arrayT[])
{
    printf("Content of Array: \n");
    for(int i = 0; i < MAX; i++)
    {
        printf("Address: %d has value: %d\n", &arrayT[i], arrayT[i]);
    }
}

void makeArray(int arrayT[])
{
    for(int i = 0; i < MAX; i++)
    {
        arrayT[i] = i*7;
    }
}

int main()
{
    initUSART();

    // create an array of MAX integers
    // immediately initialize with 0
    int arrayMAX[MAX] = {0,0,0,0,0};

    print_Array(arrayMAX);

    makeArray(arrayMAX);

    print_Array(arrayMAX);

    // make_Array();
}
