#include <avr/io.h>
#include <util/delay.h>
#include <usart.h>
#include <button.h>

int main()
{
    initUSART ();

    enableButton(2);

    while(1)
    {
        //buttonPushed prints 0 if button is pushed
        printf("%d",buttonPushed(2));
        buttonReleased(2);
        _delay_ms( 1000 );
    }
    return 0;
}