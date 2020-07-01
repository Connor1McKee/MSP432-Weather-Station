/* adc.c
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with the MSP432. */

#include "msp.h"
#include "adc.h"

uint8_t flag = 0;
int32_t nadc = 0;
uint32_t digital = 0;

/* ADC_get_flag
 * Allows files other than this one to access current flag value */
uint8_t ADC_get_flag() {
    return flag;
}

/* ADC_set_flag
 * Allows files other than this one to set the current flag value
 * NOTE: should only be used to set the flag low. The ADC interrupt
 *       will set the flag high. */
void ADC_set_flag(uint8_t new_flag) {
    flag = new_flag;
}

/* ADC_get_voltage
 * Returns the latest voltage read by the ADC */
uint32_t ADC_get_voltage() {
    return nadc;
}

/* ADC_get_voltage
 * Returns the latest raw digital value read by the ADC */
int32_t ADC_get_digital() {
    return digital;
}


/* ADC14_IRQHandler
 * ADC14 interrupt handler. Gets input from MEM and stores it in global
 * voltage value. */
void ADC14_IRQHandler() {
    /* set the flag high */
    flag = 1;

    /* write the value to digital */
    digital = ADC14 -> MEM[0];

    /* convert the value to a voltage (stored globally) */
    nadc = digital * ADC_VOLT_SLOPE - ADC_VOLT_Y_INT;
    if (nadc <= 0) {
        nadc = 0;
    }
}

/* ADC_init
 * Initializes the ADC14 on the MSP432. Samples every 16 clocks.
 * This init function uses P4.3 (A10) as the ADC input, and uses
 * memory control register 0. */
void ADC_init() {
    /* reset the ENC bit to allow configuration */
    ADC14 -> CTL0 &= ~ADC14_CTL0_ENC;

    /* adjust settings of the ADC14 */
    ADC14 -> CTL0 = ADC14_CTL0_SHP               /* use internal sample timer */
                   | ADC14_CTL0_SSEL__MCLK       /* use MCLK as clock source */
                   | ADC14_CTL0_SHT1__16         /* sample every 16 clocks */
                   | ADC14_CTL0_ON;              /* turn on the ADC14 */

    /* set 14-bit resolution, per ADC spec*/
    ADC14 -> CTL1 = ADC14_CTL1_RES_3;

    /* memory control register */
    ADC14 -> MCTL[0] = ADC14_MCTLN_INCH_10; /* select A10 */
    ADC14 -> IER0 = ADC14_IER0_IE0;         /* write to MEM[0] */

    /* set the ENC bit to enable the ADC14 */
    ADC14 -> CTL0 |= ADC14_CTL0_ENC;

    /* enable interrupts */
    NVIC -> ISER[0] = (1<<(ADC14_IRQn & 31));
    __enable_irq();

    /* use P4.3 as A10 */
    A10_PORT -> SEL0 |= A10_PIN;   /* change pin function to A10 */
    A10_PORT -> SEL1 |= A10_PIN;
    A10_PORT -> DIR |= A10_PIN;    /* set the pin as an input */
    A10_PORT -> OUT |= A10_PIN;    /* initialize P4.3 as on */

    /* start conversion */
    ADC14 -> CTL0 |= ADC14_CTL0_SC;
}

