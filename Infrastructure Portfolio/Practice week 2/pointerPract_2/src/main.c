#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

#include <stdio.h>

char arr[] = { 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\0' } ;

void arrchar1(char *arr, int pos)
{
  printf("[%d]: %c\n", pos, arr[pos] );
}

void arrchar2(char *arr, int pos)
{
                            // THIS IS THE POINTER, 
                            // pointer of arr (*arr) is the start of the array
  printf("[%d]: %c\n", pos, *(arr + pos)); 
}

int main()
{
    initUSART();

    printf("size of arr: %d\n", (int) sizeof(arr));
    
    arrchar1(arr, 6);

    arrchar2(arr, 6);

    return 0;
}