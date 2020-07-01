/* bmp280.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with the MSP432. */
#include "msp.h"

#ifndef BMP280_H_
#define BMP280_H_

/* SPI pin definitions */
#define CS_PORT P3
#define CS_BIT  BIT6

#define SPI_PORT P1
#define CLK_BIT BIT5
#define MOSI_BIT BIT6
#define MISO_BIT BIT7

/* BMP Address definitions */
#define CTRL_ADDR 0xF4
#define PRESS_MSB_ADDR 0xF7
#define PRESS_LSB_ADDR 0xF8
#define PRESS_XLSB_ADDR 0xF9
#define TEMP_MSB_ADDR 0xFA
#define TEMP_LSB_ADDR 0xFB
#define TEMP_XLSB_ADDR 0xFC
#define CALIB_START_ADDR 0x88
#define CALIB_ADDR_WIDTH 24
#define CALIB_DATA_WIDTH 12

/* BMP calibration table indices */
#define DIG_T1 0
#define DIG_T2 1
#define DIG_T3 2
#define DIG_P1 3
#define DIG_P2 4
#define DIG_P3 5
#define DIG_P4 6
#define DIG_P5 7
#define DIG_P6 8
#define DIG_P7 9
#define DIG_P8 10
#define DIG_P9 11


/* BMP modes */
#define ULTRA_LOW_RES 0x27
#define LOW_RES 0x4B

#define PRESSURE_READ 1
#define TEMPERATURE_READ 0

/* Function Prototypes */
int32_t read_from_BMP(uint8_t mode);
void SPI_init();
void BMP_init();
uint32_t compensate_pressure(int32_t uncomp_press);
int32_t compensate_temp(int32_t uncomp_temp);



#endif /* BMP280_H_ */
