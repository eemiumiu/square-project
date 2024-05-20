#include <avr/io.h> //this library includes the definitions of the registers (DDRB, PORTB, ...)
#include <util/delay.h> //Library containing the delay (~sleep) function
#include <usart.h>

int main (void) {
   initUSART ();
   // rest of your code in which you can now just use printf.
}