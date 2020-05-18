/* adc.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

#ifndef ADC_H_
#define ADC_H_

/* port definitions */
#define A10_PORT P4
#define A10_PIN BIT3

/* calculation stuff */
#define ADC_VOLT_SLOPE 206
#define ADC_VOLT_Y_INT 75000

/* function prototypes */
uint8_t ADC_get_flag();
void ADC_set_flag(uint8_t new_flag);
uint32_t ADC_get_voltage();
void ADC_send_to_UART();
void ADC_init();

#endif /* ADC_H_ */
