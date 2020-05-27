/*
 * main.c
 *
 *  Created on: May 26, 2020
 *      Author: Connor
 */

#include "bmp280.h"
#include "msp.h"
#include "delay.h"

void main(void){
    uint32_t pressure_data, temperature_data;

    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    /* Initialize SPI */
    SPI_init();

    /* Send initial configuration data to BMP */
    BMP_init();

    while(1){
        delay_us(50000);

        pressure_data = read_from_BMP(PRESSURE_READ);
        temperature_data = read_from_BMP(TEMPERATURE_READ);
    }


}

