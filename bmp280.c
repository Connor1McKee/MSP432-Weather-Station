/* bmp280.c
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with the MSP432. */


#include "bmp280.h"
#include "msp.h"

int8_t calibration_data[CALIB_DATA_WIDTH];
int32_t t_fine;


/* SPI_init
 * initializes SPI communication on the MSP432. */
void SPI_init(void) {

    /* first reset the EUSCI */
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_SWRST;

    /* next, set bits to do the following:
     * polarity as active low
     * enable synchronous mode
     * use the SMCLK as the clock source
     * set the MSB first selector
     * set as the master */
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_SWRST
                      |  EUSCI_B_CTLW0_CKPL
                      |  EUSCI_B_CTLW0_SYNC
                      |  EUSCI_B_CTLW0_UCSSEL_2
                      |  EUSCI_B_CTLW0_MSB
                      |  EUSCI_B_CTLW0_MST;

    /* set the CS bit as an output */
    CS_PORT -> SEL0 &= ~CS_BIT;
    CS_PORT -> SEL1 &= ~CS_BIT;
    CS_PORT -> DIR |= CS_BIT;
    CS_PORT -> OUT |= CS_BIT;

    /* set the appropriate SPI port bits to run in primary mode */
    SPI_PORT -> SEL0 |= (CLK_BIT | MOSI_BIT | MISO_BIT);
    SPI_PORT -> SEL1 &= ~(CLK_BIT | MOSI_BIT | MISO_BIT);

    /* set CLK and MOSI as outputs */
    SPI_PORT -> DIR |= (CLK_BIT | MOSI_BIT);
    SPI_PORT -> OUT |= (CLK_BIT | MOSI_BIT);

    /* set MISO as input */
    SPI_PORT -> DIR &= ~MISO_BIT;
    SPI_PORT -> REN &= ~MISO_BIT;

    /* clear reset */
    EUSCI_B0 -> CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);

    return;
}

void BMP_init(){
    /* configure BMP280 with the following settings:
     *              Temperature Oversampling x2
     *              Pressure Oversampling x2
     *              Normal Power Mode
     */

    send_to_BMP(CTRL_ADDR, LOW_RES);

    /* Get calibration data for temp and pressure compensation */
    fetch_calibration_data();

    return;
}

void fetch_calibration_data(){
    int i;
    int8_t calibration_value;
    uint8_t addr = CALIB_START_ADDR;

    /* Populate the calibration table */
    for(i = 0; i < CALIB_DATA_WIDTH; i++){
        calibration_value = read_calibration_value(addr);
        calibration_data[i] = calibration_value;

        /* increment addr by two for next LSB/MSB pair */
        addr += 2;
    }

}

uint16_t read_calibration_value(uint8_t addr_lsb){
    uint8_t lsb, msb;
    uint16_t dig;

    /* Data format on chip: sequential 8bit LSB/MSB pairs */

    /* set the CS bit low for reading/writing to BMP */
    CS_PORT -> OUT &= ~(CS_BIT);

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Transmit control byte */
    EUSCI_B0 -> TXBUF = addr_lsb;

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Wait for input buffer to fill */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG));

    /* Store LSB input */
    lsb = EUSCI_B0 -> RXBUF;


    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Transmit control byte. Add one for MSB address */
    EUSCI_B0 -> TXBUF = addr_lsb + 1;

    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG));

    /* Store MSB input */
    msb = EUSCI_B0 -> RXBUF;

    /* End transmission */
    CS_PORT -> OUT |= CS_BIT;

    dig = ((msb << 8) | lsb);

    return dig;
}



void send_to_BMP(uint8_t addr, uint8_t data){
    /* Set control for writing BIT7 = 0 */
    addr &= ~BIT7;

    /* set the CS bit low for writing to BMP */
    CS_PORT -> OUT &= ~(CS_BIT);

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Transmit control byte */
    EUSCI_B0 -> TXBUF = addr;

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Transmit data/config byte */
    EUSCI_B0 -> TXBUF = data;

    /* wait for transmit to complete */
    while(!(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG));

    /* set the CS bit high to end write*/
    CS_PORT -> OUT |= CS_BIT;

    return;

}

int32_t read_from_BMP(uint8_t mode){
    uint8_t msb, lsb, xlsb, addr;
    int32_t input = 0;

    /* Configure for reading 8 bits MSB Pressure/Temp data BIT7 = 1 */
    if(mode == PRESSURE_READ){
        addr = PRESS_MSB_ADDR | BIT7;
    } else {
        addr = TEMP_MSB_ADDR | BIT7;
    }

    /* set the CS bit low for reading/writing to BMP */
    CS_PORT -> OUT &= ~(CS_BIT);

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Transmit control byte */
    EUSCI_B0 -> TXBUF = addr;

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Wait for input buffer to fill */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG));

    /* Store MSB input */
    msb = EUSCI_B0 -> RXBUF;


    /* Configure next address for LSB */
    if(mode == PRESSURE_READ){
        addr = PRESS_LSB_ADDR | BIT7;
    } else {
        addr = TEMP_LSB_ADDR | BIT7;
    }

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Transmit control byte */
    EUSCI_B0 -> TXBUF = addr;

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Wait for input buffer to fill */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG));

    /* Store LSB input */
    lsb = EUSCI_B0 -> RXBUF;


    /* Configure next address for XLSB */
    if(mode == PRESSURE_READ){
        addr = PRESS_XLSB_ADDR | BIT7;
    } else {
        addr = TEMP_XLSB_ADDR | BIT7;
    }

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Transmit control byte */
    EUSCI_B0 -> TXBUF = addr;

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* Wait for input buffer to fill */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG));

    /* Store MSB input */
    xlsb = EUSCI_B0 -> RXBUF;

    /* set the CS bit high to end transmission */
    CS_PORT -> OUT |= CS_BIT;

    /* Combine input */

    input = ((msb << 16) | (lsb << 8) | xlsb);

    input >>= 4;

    return input;
}

/* Following the temperature conversion outlined by Bosch */
//int32_t compensate_temp(int32_t uncomp_temp){
//    int32_t var1, var2, T;
//
//    var1 = ((uncomp_temp>>3) - ((int32_t)calibration_data[DIG_T1]<<1)) *
//            ((int32_t) calibration_data[DIG_T2]) >> 11;
//    var2 = ((((uncomp_temp>>4) - ((int32_t)calibration_data[DIG_T1])) *
//            ((uncomp_temp>>4) - ((int32_t)calibration_data[DIG_T1]))) >> 12) *
//                    ((int32_t)calibration_data[DIG_T3]) >> 14;
//    t_fine = var1 + var2;
//    T = (t_fine * 5 + 128) >> 8;
//    //T = (var1 + var2)/5120;
//    return T;
//}

int32_t compensate_temp(int32_t uncomp_temp){
    int32_t var1, var2, T;

    var1 = (((double)uncomp_temp)/16384.0 -
            ((double)calibration_data[DIG_T1])/1024.0) *
                    ((double)calibration_data[DIG_T2]);
    var2 = ((((double)uncomp_temp)/131072.0 -
                ((double)calibration_data[DIG_T1])/8192.0) *
                    (((double)uncomp_temp)/131072.0 -
                            ((double)calibration_data[DIG_T1])/8192.0)) *
                                    ((double)calibration_data[DIG_T3]);
    t_fine = ((int32_t)(var1 + var2));
    T = ((var1 + var2)/5120.0);
    return T;
}



uint32_t compensate_pressure(int32_t uncomp_press){
    int64_t var1, var2, p;

    var1 = ((int64_t) t_fine) -128000;
    var2 = var1 * var1 * (int64_t)calibration_data[DIG_P6];
    var2 = var2 + ((var1 * (int64_t)calibration_data[DIG_P5]) << 17);
    var2 = var2 + (((int64_t)calibration_data[DIG_P4]) << 35);
    var1 = ((var1 * var1 * (int64_t)calibration_data[DIG_P3]) >> 8) +
            ((var1 * (int64_t)calibration_data[DIG_P2]) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calibration_data[DIG_P1]) >> 33;

    if(var1 == 0){
        return 0;
    }

    p = 1048576 - uncomp_press;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calibration_data[DIG_P9]) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calibration_data[DIG_P8]) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)calibration_data[DIG_P7]) << 4);

    return (uint32_t) p;
}



