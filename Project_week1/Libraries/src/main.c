#include <Arduino.h>
#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>
#include <led.h>

// !!!!
// I believe for a teacher to test this, they should change the  
// path of the lib_extra_dirs in platformio.ini, because 
// on Mac and on Windows, path sidebars are diffrent ( / and \ ). 

int main()
{
  initUSART();
  printf ("Testing led library functions\n");

  enableLed(1);
  enableMultipleLeds(0b00001101); 
  enableAllLeds();

  while(1)
  {
    _delay_ms(1000);
    lightDownAllLeds();
    _delay_ms(1000);
     printf ("Turning led 1 on\n");
    lightUpLed(1);
    _delay_ms(1000);
     printf ("Turning led 1 off\n");
    lightDownLed(1);
    _delay_ms(1000);

    // Testing 1.11, enableMultipleLeds and enableAllLeds
    printf ("Turning led 0, 2 and 3 on\n");
    lightUpMultipleLeds(0b00001101);
    _delay_ms(1000);
    printf ("Turning led 0, 2 and 3 off\n");
    lightDownMultipleLeds(0b00001101);
    _delay_ms(1000);
    printf ("Turning all leds on\n");
    lightUpAllLeds();
    _delay_ms(1000);
    printf ("Turning all off\n");
    lightDownAllLeds();
    _delay_ms(1000); 
    
    // Testing 1.12, dimLed, fadeInLed and fadeOutLed
    dimLed(0, 15, 100); // led_num, percentage, duration(ms)
    fadeInLed(1,100);
    fadeOutLed(1,100);
  }

  return 0;
}