/* LCD.c
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with the 1602A 16x2 LCD display module. */

#include "msp.h"
#include "LCD.h"
#include "delay.h"

/* LCD_write_command
 * Writes the passed in command to the LCD. */
void LCD_write_command(uint8_t command){

    /* clear out the data bits */
    LCD0->OUT &= ~(DB7 | DB6 | DB5 | DB4);
    /* set the bits high that correspond to the inputted command */
    LCD0->OUT |= (0xF0 & command);
    /* set RW and RS low, to input command */
    LCD0->OUT &= ~RW;
    LCD0->OUT &= ~RS;

    /* pulse the enable bit */
    delay_us(1000);
    LCD0->OUT |= EN;
    delay_us(1000);
    LCD0->OUT &= ~EN;

    /* figure the delay for next command/write */
    delay_us(2000);
}

/* LCD_nibble_write_command
 * To be used when nibble mode is enabled -- sends the highest four bits
 * of the inputted command, then the lower four bits */
void LCD_nibble_write_command(uint8_t command) {
    /* write the highest four bits, then the lowest four bits. */
    LCD_write_command(command & 0xF0);
    delay_us(1000);
    LCD_write_command(command << 4);
    delay_us(1000);
}

/* LCD_write
 * Writes a character to the LCD. */
void LCD_write(uint8_t data){

    /* clear out the data bits */
    LCD0->OUT &= ~(DB7 | DB6 | DB5 | DB4);
    /* set the bits high that correspond to the inputted data */
    LCD0->OUT |= (0xF0 & data);
    /* set RW low and RS high, to signify data input */
    LCD0->OUT &= ~RW;
    LCD0->OUT |= RS;

    /* pulse the enable bit */
    delay_us(1000);
    LCD0->OUT |= EN;
    delay_us(1000);
    LCD0->OUT &= ~EN;

    /* delay for next command/data */
    delay_us(2000);
}

/* LCD_nibble_write_command
 * To be used when nibble mode is enabled -- sends the highest four bits
 * of the inputted character, then the lower four bits */
void LCD_nibble_write(uint8_t data){
    /* write the highest four bits, then the lowest four bits. */
    LCD_write(data & 0xF0);
    delay_us(50);
    LCD_write(data << 4);
    delay_us(50);
}

/* LCD_write_string
 * Takes in a string and writes it to the LCD at its current cursor location */
void LCD_write_string(char* string) {
    int i;

    /* loop through the inputted string, writing each character, until we hit
     * the null terminator. */
    for (i = 0; string[i] != '\0'; i++) {
        LCD_nibble_write(string[i]);
    }
}

/* LCD_go_to_line
 * Set the cursor of the LCD to the inputted line
 * (LCD is only two lines tall, so input should be either 1 or 2 */
void LCD_go_to_line(uint8_t line)
{
    /* set the DDRAM address to start writing at the inputted line. */
    if (line == 2) {
        LCD_nibble_write_command(DDRAM_ADDRESS_SET | 0x28);
    }
    else {
        LCD_nibble_write_command(DDRAM_ADDRESS_SET | 0x00);
    }
}

/* LCD_reset
 * Pretty self-explanatory. */
void LCD_reset()
{
    /* pretty self explanatory! */
    LCD_clear();
    LCD_home();
}

/* LCD_home
 * This function returns the cursor to the upper left corner */
void LCD_home()
{
    LCD_nibble_write_command(DISPLAY_CURSOR_HOME);
}

/* LCD_clear
 * This function will clear the screen */
void LCD_clear()
{
    LCD_nibble_write_command(CLEAR_DISPLAY);
}

/* LCD_turn_off_cursor
 * This function will turn off the cursor */
void LCD_turn_off_cursor() {
    LCD_nibble_write_command(DISPLAY_CONTROL | DISPLAY_ON);
}

/* LCD_turn_on_cursor
 * This function will turn on the cursor */
void LCD_turn_on_cursor() {
    LCD_nibble_write_command(DISPLAY_CONTROL | DISPLAY_ON |
                             CURSOR_DISPLAY_ON | CURSOR_BLINK_ON);
}

/* LCD_init
 * Initializes the LCD with the following settings:
 *   - function set is dual line
 *   - display on, with cursor blinking
 *   - display cleared
 *   - entry mode increment */
void LCD_init(){
    /* config LCD pins as simple IO */
    LCD0->SEL1 &= ~(RS | RW | EN | DB4 | DB5 | DB6 | DB7);
    LCD0->SEL0 &= ~(RS | RW | EN | DB4 | DB5 | DB6 | DB7);

    /* configure all of the LCD pins as output */
    LCD0->DIR |= (RS | RW | EN | DB4 | DB5 | DB6 | DB7);

    /* delay before initialization, for boot time */
    delay_us(100000);

    /* wake up! (sequence from data sheet) */
    LCD_write_command(0x00);
    LCD_write_command(0x00);
    LCD_write_command(0x00);
    LCD_write_command(0x30);
    delay_us(100000);
    LCD_write_command(0x30);
    delay_us(100000);
    LCD_write_command(0x20);
    delay_us(100000);

    /* set function set for dual line */
    LCD_nibble_write_command(FUNCTION_SET | DL_NUMBER);

    /* turn on the display, with a blinking cursor */
    LCD_nibble_write_command(DISPLAY_CONTROL | DISPLAY_ON |
                            CURSOR_DISPLAY_ON | CURSOR_BLINK_ON);

    /* clear the display */
    LCD_nibble_write_command(CLEAR_DISPLAY);

    /* change entry mode to increment */
    LCD_nibble_write_command(ENTRY_MODE_SET | INCREMENT_DECREMENT);

}
