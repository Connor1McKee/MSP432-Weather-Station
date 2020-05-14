#include "msp.h"
#include "dht.h"
#include "lcd.h"
#include "delay.h"
#include "raindrops.h"

#include <stdio.h>
#include <stdlib.h>

#define DEGREE_CHARACTER 223

/**
 * main.c
 */
void main(void) {
    char buffer[100];

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	set_DCO(FREQ_12_MHZ);

	DHT_init();
	LCD_init();
	RD_init();

	LCD_turn_off_cursor();

	while(1) {
//	    DHT_read_data();
//	    if (DHT_check_checksum()) {
//	        LCD_go_to_line(1);
//
//	        sprintf(buffer, "Humidity = %2d%%", dht_data[0]);
//	        LCD_write_string(buffer);
//	        LCD_go_to_line(2);
//	        sprintf(buffer, "Temp = %3d %cC", dht_data[2], DEGREE_CHARACTER);
//            LCD_write_string(buffer);
//	    }
//	    else {
//	        LCD_go_to_line(1);
//	        LCD_write_string("Got bad data I guess");
//	    }
//
//	    DHT_reset_data();
//	    delay_us(1000000);
	    LCD_go_to_line(1);
	    if(RD_read_digital()) {
	        LCD_write_string("Rain detected!    ");
	    }
	    else {
	        LCD_write_string("Rain not detected.");
	    }
	    delay_us(1000);

	}
}
