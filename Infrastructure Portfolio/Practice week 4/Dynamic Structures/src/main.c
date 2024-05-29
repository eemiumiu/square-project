#include <avr/io.h>
#include <stdlib.h>
#include <usart.h>
#include <util/delay.h>
#include <string.h>

typedef struct
{
    int value;
    char* suit; //Note: pointer, because we want to reserve just enough
} CARD;

void fullDeck( CARD* deck )
{
    char suits[4][10] = { "hearts","diamonds","spades","clubs" };
    int index;
    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 13; j++ )
        {
            index = ( i * 13 ) + j;

            //DONE: use malloc to reserve just enough space for the suit
            deck[index].suit = malloc(strlen(suits[i]) + 1); // +1 for the null terminator
            if (deck[index].suit == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            //DONE: copy the color to the allocated address space
            strcpy(deck[index].suit, suits[i]);
            //DONE: set the card's value
            deck[index].value = (j + 1);
        }
    }
}

//BEWARE: the address of a CARD comes in:
void showCard( CARD* theCard )
{
        //DONE: update the following code:
    switch (theCard->value)
    {
        // Special cards:
        case 1:
            printf("ace of %s", theCard->suit);
            break;
        case 11:
            printf("jack of %s", theCard->suit);
            break;
        case 12:
            printf("queen of %s", theCard->suit);
            break;
        case 13:
            printf("king of %s", theCard->suit);
            break;
        default:    // All other cards:
            printf("%d of %s", theCard->value, theCard->suit);
    }
}

//BEWARE: the return value is a pointer!
CARD* drawCard( CARD deck[] )
{
    int randm = rand() % 52;
    //DONE: return the address of the random card
    return &deck[randm];
}

int main()
{
    initUSART();
    //DONE: use calloc to make the deck of cards
    CARD* deck = (CARD*)calloc(52, sizeof(CARD));

    fullDeck( deck );

    //DONE: update the following code so that it compiles
    for ( int i = 0; i < 10; i++ )
    {
        CARD* card = drawCard( deck );
        showCard( card );
        printf( "\n" );
    }

    //DONE: free all allocated memory:
    for (int i = 0; i < 52; i++)
    {
        free(deck[i].suit);
    }
    free(deck); // Free the deck itself
    
    return 0;
}