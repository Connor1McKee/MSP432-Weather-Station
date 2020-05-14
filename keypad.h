/* keypad.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

/* define pin values corresponding to keypad */
#define ROWS P2
#define COLS P5

#define R1 BIT4
#define R2 BIT5
#define R3 BIT6
#define R4 BIT7

#define C1 BIT0
#define C2 BIT1
#define C3 BIT2

/* function prototypes */
void keypad_init();
char keypad_get_key();

#endif /* KEYPAD_H_ */
