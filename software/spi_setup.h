/* 
 * File:   spi2_setup.h
 * Author: Alex
 *
 * Created on April 5, 2019, 12:02 PM
 */


#ifndef SPI_H
#define	SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  

void spi_setup();
uint32_t spi_read_data();
uint8_t spi_write_reg(uint8_t address, uint8_t data);
uint8_t spi_write_cmd(uint8_t cmd);
uint8_t spi_read_reg(uint8_t address);

#endif


#ifdef	__cplusplus
}
#endif

