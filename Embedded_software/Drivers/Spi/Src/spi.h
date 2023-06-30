#ifndef __SPI_H_
#define __SPI_H_

#include <stdint.h>

#define SPI_CS_ENABLE   (1U)
#define SPI_CS_DISABLE  (0U)

/*
 * Functions prototypes
 */
void Spi_Init(void);
void Spi_TxRx(uint8_t length, uint8_t* tx_buffer, uint8_t* rx_buffer);

#endif