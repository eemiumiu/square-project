#include <avr/io.h> // ATmega328 pins, def of the registers (DDRB, PORTB, ...)
#include <usart.h>  // for Serial Monitor
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <led.h>
#include <button.h>
#include <buzzer.h>

#include <stdbool.h>

int main()
{
    enableBuzzer();
    _delay_ms(1000);

    playTone(523.250, 300);
    _delay_ms(1000);

    disableBuzzer();
}
