/* raindrops.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

#ifndef RAINDROPS_H_
#define RAINDROPS_H_

/* macro defintions */
#define RD_PORT P5
#define RD_DIGITAL_PIN BIT5
/* analog will be read with ADC on A10 (P4.3) */
#define RD_ANALOG_PORT A10_PORT
#define RD_ANALOG_PIN A10_PIN

#define RAIN_DETECTED 1
#define NO_RAIN 0

/* function prototypes */
void RD_init();
uint8_t RD_read_digital();

#endif /* RAINDROPS_H_ */
