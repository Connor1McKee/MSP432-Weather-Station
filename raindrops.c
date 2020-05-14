/* raindrops.c
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with the MH-RD Raindrops module. */

#include "msp.h"
#include "adc.h"
#include "raindrops.h"

uint8_t RD_read_analog() {
    return 0; /* TODO */
}

uint8_t RD_read_digital() {
    /* if our digital pin is high, there is no rain */
    if (RD_PORT -> IN & RD_DIGITAL_PIN) {
        return NO_RAIN;
    }
    /* if not, there is rain detected. */
    return RAIN_DETECTED;

}

void RD_init() {
    /* set digital output of module to simple IO, input */
    RD_PORT -> SEL0 &= ~RD_DIGITAL_PIN;
    RD_PORT -> SEL0 &= ~RD_DIGITAL_PIN;
    RD_PORT -> DIR &= ~RD_DIGITAL_PIN;

    /* set analog output of module (TODO) */
    /* ADC_init(); */
}
