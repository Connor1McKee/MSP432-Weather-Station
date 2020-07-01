#include "msp.h"
#include "adc.h"
#include "dht.h"
#include "lcd.h"
#include "delay.h"
#include "raindrops.h"
#include "keypad.h"
#include "math.h"

#include <stdio.h>
#include <stdlib.h>

#define DEGREE_CHARACTER 223
#define LOOP_DELAY 1000


/**
 * main.c
 */
void main(void) {
    char buffer[100];

	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	/* Variables to hold wind speed, and keypad characters */
	char key;
	float  wind_mph;
	char current = 0xFF;
	int32_t raw_windspd = -1;

	set_DCO(FREQ_12_MHZ);

	/* Initialize all sensors/modules used */
	ADC_init();
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
                /* if "1" read, display Humidity data */
                if (current == '1') {
                    sprintf(buffer, "Humidity = %2d%%", dht_data[0]);
                }
                /*  if "2" read, display Temperatue data */
                else if (current == '2') {
                    sprintf(buffer, "Temp = %3d %cC", dht_data[2],
                                DEGREE_CHARACTER);
                }
                LCD_write_string(buffer);
            }
	    }
	    /* when 3 pressed, display rain drop (RD) sensor data */
	    else if (current == '3') {
            LCD_go_to_line(1);
            LCD_write_string("Rain detected?");
            LCD_go_to_line(2);

            /* Read RD digital value to detect presence of moisture */
            if(RD_read_digital()) {
                LCD_write_string("Yes! :o");
            }
            else {
                LCD_write_string("Nope :)");
            }
	    }
	    /* when 4 pressed, display wind sensor data converted by the ADC */
	    else if (current == '4'){
	        LCD_go_to_line(1);
	        LCD_write_string("Wind   Speed    ");
	        LCD_go_to_line(2);
	        if (raw_windspd == -1){
	            sprintf(buffer, "Data not ready..");
	        } else {
	            sprintf(buffer, "%4.3f  MPH       ", wind_mph);
	        }

	        LCD_write_string(buffer);

	        if(ADC_get_flag()){
	            /* reset the flag for next read */
	            ADC_set_flag(0);

	            /* get the valid 10-bit raw value from the ADC */
	            raw_windspd = ADC_get_digital() & 0x3FF;

	            /* wind speed conversion to MPH */
	            wind_mph = powf(((raw_windspd - TEMP_COMP) / WIND_DIVISOR),
	                               WIND_POWER);

	            /* Scale wind speed */
	            wind_mph *= SCALE_FACTOR;

	            /* Start ADC conversion again */
	            ADC14 -> CTL0 |= ADC14_CTL0_SC;
	        }

	    }
	    /* when any other characters are input, wait for next key pressed */
	    else {
	        LCD_go_to_line(1);
            LCD_write_string("Waiting for");
            LCD_go_to_line(2);
            LCD_write_string("input...");
	    }

	    /* Delay on each loop to slow down measurement times */
        delay_us(LOOP_DELAY);
	}
}
