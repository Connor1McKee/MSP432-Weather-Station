#include "msp.h"
#include "dht.h"
#include "lcd.h"
#include "delay.h"
#include "raindrops.h"
#include "keypad.h"

#include <stdio.h>
#include <stdlib.h>

#define DEGREE_CHARACTER 223
#define LOOP_DELAY 1000
#define DHT_DELAY 1000000

/**
 * main.c
 */
void main(void) {
    char buffer[100];

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	char key;
	char current = 0xFF;

	set_DCO(FREQ_12_MHZ);

	DHT_init();
	LCD_init();
	RD_init();
	keypad_init();

	LCD_turn_off_cursor();

	/* loop delays 1 ms each loops */
	while(1) {

	    key = keypad_get_key();

	    /* if we have pressed a new key, clear the display. */
	    if (current != key && key != 0xFF) {
	        current = key;
	        LCD_clear();
	    }

	    /* when 1 or 2 is pressed, switch to DHT data on screen*/
	    if (current == '1' || current == '2') {
            DHT_read_data();
            if (DHT_check_checksum()) {
                LCD_go_to_line(1);
                if (current == '1') {
                    sprintf(buffer, "Humidity = %2d%%", dht_data[0]);
                }
                else if (current == '2') {
                    sprintf(buffer, "Temp = %3d %cC", dht_data[2], DEGREE_CHARACTER);
                }
                LCD_write_string(buffer);
            }
	    }

	    else if (current == '3') {
            LCD_go_to_line(1);
            LCD_write_string("Rain detected?");
            LCD_go_to_line(2);
            if(RD_read_digital()) {
                LCD_write_string("Yes! :o");
            }
            else {
                LCD_write_string("Nope :)");
            }
	    }

	    else {
	        LCD_go_to_line(1);
            LCD_write_string("Waiting for");
            LCD_go_to_line(2);
            LCD_write_string("input...");
	    }
        delay_us(LOOP_DELAY);
	}
}
