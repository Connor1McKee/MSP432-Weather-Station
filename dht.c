/* dht.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with the DHT11 temperature sensor. */

#include "msp.h"
#include "delay.h"
#include "dht.h"

void DHT_init() {
    /* when power is supplied to the sensor, do not send any instruction to the
     * sensor in within one second in order to pass the unstable status. */
    delay_us(1000000);

    /* set the data pin as an input */
    DHT_PORT -> SEL0 &= ~DHT_PIN;
    DHT_PORT -> SEL1 &= ~DHT_PIN;
    DHT_PORT -> DIR &= ~DHT_PIN;
    DHT_PORT -> OUT &= ~DHT_PIN;
}

uint8_t DHT_check_checksum(uint8_t dht_data[], uint8_t j) {
    uint32_t dht_data_added = 0;
    uint8_t i;

    /* sum up the values to compare to our checksum */
    for (i = 0; i < DHT_DATA_SIZE-1; i++) {
        dht_data_added += dht_data[i];
    }
    dht_data_added &= 0xFF;

    /* check that all data was populated and checksum equals our sum */
    if ((j >= 40) && dht_data[DHT_CHECKSUM_INDEX] == dht_data_added) {
        return GOOD_DATA;
    }
    return BAD_DATA;
}

uint8_t DHT_populate_data_table(uint8_t* dht_data[]) {
    uint8_t last_state = 1;
    uint8_t count = 0;
    uint8_t i;
    uint8_t j = 0;

    /* detect change and read data */
    for (i=0; i < MAXTIMINGS; i++) {
        count = 0;

        /* count up while nothing has changed */
        while (last_state == (DHT_PORT -> IN & DHT_PIN)) {
            count++;
            delay_us(1);
            /* if we have delayed the max time, there was no change */
            if (count == MAX_COUNT) {
                break;
            }
        }

        /* read in the current input value */
        last_state = DHT_PORT -> IN & DHT_PIN;

        if (count == MAX_COUNT) {
            break;
        }

        /* ignore the first 3 transitions */
        if ((i >= 4) && (i % 2 == 0)) {
            /* put this bit of data in the corresponding data spot */
            *dht_data[j / DHT_INDEX_SIZE] <<= 1;
            if (count > 16)
                *dht_data[j / DHT_INDEX_SIZE] |= 1;
            j++;
        }
    }

    return j;
}

uint8_t* DHT_read_data() {
    uint8_t dht_data[DHT_DATA_SIZE] = {0, 0, 0, 0, 0};
    uint8_t i;

    /* set the pin as an output and pull down pin for at least 18 ms */
    DHT_PORT -> DIR |= DHT_PIN;
    DHT_PORT -> OUT &= ~DHT_PIN;
    delay_us(18000);

    /* pull the pin up for 20-40us to wait for the sensor's response */
    DHT_PORT -> OUT |= DHT_PIN;
    delay_us(20);

    /* set the pin as an input to prepare for reading. */
    DHT_PORT -> DIR &= ~DHT_PIN;

    /* populate our data table */
    i = DHT_populate_data_table(&dht_data);

    /* return the data if the checksum lines up, and NULL if not */
    if (DHT_check_checksum(dht_data, i)) {
        return dht_data;
    }
    return (uint8_t*) 0;
}

