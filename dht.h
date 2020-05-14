/* dht.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

/* NOTE: DHT11 requires one second after given power before initializing. */

#ifndef DHT_H_
#define DHT_H_
;
#define DHT_PORT P6
#define DHT_PIN BIT4

#define MAX_TIME 85

#define GOOD_DATA 1
#define BAD_DATA 0

#define DHT_DATA_SIZE 5
#define DHT_INDEX_SIZE 8
#define DHT_CHECKSUM_INDEX 4

#define MAX_COUNT 255
#define HIGH_BIT_COUNT 12

void DHT_init();
void DHT_read_data();
void DHT_reset_data();
uint8_t DHT_check_checksum();

extern int dht_data[DHT_DATA_SIZE];

#endif /* DHT_H_ */
