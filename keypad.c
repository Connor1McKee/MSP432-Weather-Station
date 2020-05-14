/* keypad.c
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with any generic keypad. */

#include "msp.h"
#include "keypad.h"

/* keypad_init
 * To be run before using the keypad, initializes the keypad for use */
void keypad_init()
{
    /* set the pins as simple IO */
    COLS -> SEL0 &= ~(C1 | C2 | C3);
    COLS -> SEL1 &= ~(C1 | C2 | C3);
    ROWS -> SEL0 &= ~(R1 | R2 | R3 | R4);
    ROWS -> SEL1 &= ~(R1 | R2 | R3 | R4);

    /* set column bits to be outputs */
    COLS -> DIR |= (C1 | C2 | C3);
    COLS -> OUT &= ~(C1 | C2 | C3);

    /* set row bits to be inputs, with a pull-up resistor */
    ROWS -> DIR &= ~(R1 | R2 | R3 | R4);
    ROWS -> REN |= (R1 | R2 | R3 | R4);
    ROWS -> OUT &= ~(R1 | R2 | R3 | R4);
}

/* keypad_get_key
 * returns pressed key as a character, or 0xFF if nothing was being pressed */
char keypad_get_key()
{
    int i = 0;
    uint8_t row = 0;

    /* an array that corresponds to possible inputted values */
    char data[4][3] = { {'1', '2', '3'},
                        {'4', '5', '6'},
                        {'7', '8', '9'},
                        {'*', '0', '#'} };

    for (i = 0; i < 3; i++)
    {
        /* drive column i high */
        COLS -> OUT &= ~(C1 | C2 | C3);
        COLS -> OUT |= (BIT0 << i);
        __delay_cycles(25);

        /* get the row being pressed (if in current column) */
        row = (ROWS -> IN & 0xF0);

        /* return the value corresponding to the row/column pair being
         * pressed */
        if (row == R1) {
            return data[0][i];
        }
        else if (row == R2) {
            return data[1][i];
        }
        else if (row == R3) {
            return data[2][i];
        }
        else if (row == R4) {
            return data[3][i];
        }
    }

    /* if we get here, no input was received, so return 0xFF. */
    return 0xFF;
}
