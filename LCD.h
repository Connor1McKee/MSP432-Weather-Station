/* LCD.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

#ifndef LCD_H_
#define LCD_H_

/* Constant Definitions */
#define LCD0 P4
#define RS BIT0
#define RW BIT1
#define EN BIT2
#define DB4 BIT4
#define DB5 BIT5
#define DB6 BIT6
#define DB7 BIT7

/* All LCD Commands */
#define CLEAR_DISPLAY 1
#define DISPLAY_CURSOR_HOME 2
#define ENTRY_MODE_SET 4
#define DISPLAY_CONTROL 8
#define DISPLAY_CURSOR 16
#define FUNCTION_SET 32
#define RAM_ADDRESS_SET 64
#define DDRAM_ADDRESS_SET 128
#define READ_BUSY_AND_AC 256
#define DATA_WRITE 512
#define DATA_READ 768

/* Used with ENTRY_MODE_SET */
#define INCREMENT_DECREMENT 2 /* on: increment, off: decrement */
#define DISPLAY_SHIFT_ON 1

/* Used with DISPLAY_CONTROL */
#define DISPLAY_ON 4
#define CURSOR_DISPLAY_ON 2
#define CURSOR_BLINK_ON 1

/* Used with DISPLAY_CURSOR */
#define DISPLAY_OR_CURSOR 8 /* on: shift display, off: move cursor */
#define SHIFT_LR 4 /* on: shift right, off: shift left */

/* Used with FUCTION_SET */
#define DATA_LENGTH 16 /* on = 8-bit, off = 4-bit */
#define DL_NUMBER 8 /* on = dual line, off = single line */
#define CHARACTER_FONT 4 /* on = 5x10 dots, off = 5x8 dots */

/* Used with READ_BUSY_AND_AC */
#define BUSY_FLAG 128

/* Function Prototypes */
void LCD_init();
void LCD_home();
void LCD_clear();
void LCD_nibble_write(uint8_t data);
void LCD_write_string(char* string);
void LCD_go_to_line(uint8_t line);
void LCD_turn_on_cursor();
void LCD_turn_off_cursor();
void LCD_reset();


#endif /* LCD_H_ */
