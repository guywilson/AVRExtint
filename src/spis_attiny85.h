#ifndef _INCL_SPIS_ATTINY85
#define _INCL_SPIS_ATTINY85

/*
 * Supported SPI commands
 */
#define SPI_CMD_GET_COUNT_LSB		'L'
#define SPI_CMD_GET_COUNT_MSB		'M'
#define SPI_CMD_RESET_COUNTER		'R'

void setupSPISlave();

#endif
