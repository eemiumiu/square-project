#include <avr/io.h>
#include <usart.h>

#define NUMBER_OF_BUTTONS 3

// #define DEBUG

void enableButton( int button )
{
    initUSART();
    if ( button < 1 || button > NUMBER_OF_BUTTONS ) return;
    DDRC &= ~_BV( PC1 + (button - 1) ); // Set up DDR 
    PORTC |= _BV( PC1 + (button - 1));  // Activate pull-up 

    #ifdef DEBUG   
    printf( "DDRC: " );         
    printBinaryByte( DDRC );
    printf( "\nPORTC: " );
    printBinaryByte( PORTC );
    #endif
}

int buttonPushed( int button )
{
    // return 0 if button is pushed
    if ( button < 1 || button > NUMBER_OF_BUTTONS ) 
        return -1;
    if ( bit_is_clear( PINC, (PC1 + (button - 1)) )) 
    {
        #ifdef DEBUG
        printf( "\nPINC: " );
        printBinaryByte( PINC );
        printf( " - Button %d pressed!\n", button );
        #endif
        return 0;
    }
    return -1;
}

int buttonReleased( int button )
{
    // return 1 if button is not pushed
    if ( button < 1 || button > NUMBER_OF_BUTTONS ) 
        return -1;
    if ( !bit_is_clear( PINC, (PC1 + (button - 1)) )) 
    {
        #ifdef DEBUG
        printf( "\nPINC: " );
        printBinaryByte( PINC );
        printf( " - Button %d NOT pressed!!\n", button );
        #endif
        return 1;
    }
    return -1;
}

void enableButtonInterrupt(int button)
{
    if ( button < 1 || button > NUMBER_OF_BUTTONS ) return;
    
    PCICR |= _BV( PCIE1);

    PCMSK1 |= _BV( PC1  + (button - 1) );
}

void enableAllButtonInterrupts()
{
    PCICR |= _BV( PCIE1);
    PCMSK1 |= _BV( PC1 );
    PCMSK1 |= _BV( PC2 );
    PCMSK1 |= _BV( PC3 );
}