#include <avr/io.h>
void enableBuzzer(); // enables the buzzer
void playTone( float frequency, uint32_t duration ); // plays a tone
void disableBuzzer(); // disables the buzzer
// playMusic(**notes);