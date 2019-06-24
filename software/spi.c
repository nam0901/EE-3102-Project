/*
 * spi.c
 * 
 * This code allows us to communicate in SPI with the ADC
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "spi_setup.h"
#include "pin_mapping.h"

/*
 * Setup:
 * SP1CON:
 *      Master mode
 *      8-bit communication
 *      Data sampled at end out output
 *      Output data changes on falling edge
 *      Idle for clock is low
 *      SCLk -> 8MHz?
 * 
 * Pins:
 *      RB9 = ~RS
 *      RB10 = CLK
 *      RB11 = ~CS
 *      RB12 = SCLK
 *      RB13 = DIN
 *      RB14 = DOUT
 *      RB15 = ~DRDY
 */
void spi_setup(void){
   
    SPI_DOUT_TRIS = 0; 
    SPI_DIN_TRIS = 1;
    SPI_CS_TRIS = 0;
    SPI_SCLK_TRIS = 0;

    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR6bits.RP13R = 7;   //RB13 -> SPI1:SDO1
    RPOR6bits.RP12R = 8;   //RB12 -> SPI1:SCK1OUT
    RPINR20bits.SDI1R = 14; //RB14 -> SD11:DIN
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    SPI1CON1 = 0;
    SPI1CON1bits.MSTEN = 1;  // master mode
    SPI1CON1bits.MODE16 = 0; // eight bit mode
    SPI1CON1bits.CKE = 0;   // data on transition from active -> low
    SPI1CON1bits.CKP = 0;   //idle for clock is low
    SPI1CON1bits.SPRE = 0b111; // secondary prescaler = 2:1
    SPI1CON1bits.PPRE = 0b10;  // primary prescaler = 1:1
    SPI1STAT = 0;
    SPI1STATbits.SISEL = 0b101; // IF set when last bit is shifted out
                                // That means the SPI xfer is complete.
    SPI1STATbits.SPIEN = 1;
    
    // Disable chip select (no active transfer)
    SPI_CS_LAT = 1;
    
    // Send START command (starts the adc conversions)
    spi_write_cmd(0x08);
}

/*
  @Description
    Read data from SPI buffer.
*/
uint32_t spi_read_data()
{
    uint32_t read_val = 0;
    uint32_t read_24;
    uint32_t read_16;
    uint32_t read_8;
    uint8_t status;

    SPI_CS_LAT = 0;
    
    SPI1BUF = 0x12;// send "read_frame to SPI1 buffer    
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit   
    read_24 = SPI1BUF; // read bits 24-16 MSB first
    
    SPI1BUF = 0x12;// send "read_frame to SPI1 buffer
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit
    read_16 = SPI1BUF; // read bits 24-16 MSB first
    
    SPI1BUF = 0x12;// send "read_frame to SPI1 buffer    
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit   
    read_8 = SPI1BUF; // read bits 24-16 MSB first
    
    SPI1BUF = 0x00; //Reading an extra byte here seems to be required for the MISO signal to work properly. 
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    status = SPI1BUF;// clear SPI1 buffer using the "trash" variable
    
    SPI_CS_LAT = 1;
    
    read_val = (read_24 << 16) | (read_16 << 8) | (read_8);
    
    return read_val;
}

/* ************************************************************************** */
/** spi_write_register.c

  @Description
    Send data to SPI buffer.
 */
/* ************************************************************************** */
uint8_t spi_write_cmd(uint8_t cmd)
{   
    uint8_t trash;
    
    SPI_CS_LAT = 0;
    
    SPI1BUF = cmd;// send "write_frame" to the SPI2 buffer
    while(!SPI1STATbits.SPIRBF); // wait for the SPI2 buffer full bit 
    trash = SPI1BUF;// clear SPI2 buffer using the "trash" variable                                        

    SPI_CS_LAT = 1;
    
    return trash;
}

/*
 * spi_write_register
 * 
 * @Description
 *  writes an two eight bit data bytes to registers
 * 
 * @Params
 *  byte address - defines starting address for ADC
 *  byte number - defines number of registers + 1
 *  uint8_t config - data for config reg
 *  uint8_t ofc0 - data for ofc0 portion of reg
 * 
 */
uint8_t spi_write_reg(uint8_t address, uint8_t data) {
    
    uint8_t trash;
    
    SPI_CS_LAT = 0;
   
    SPI1BUF = 0x40 | address;
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    trash = SPI1BUF;// clear SPI1 buffer using the "trash" variable
    
    SPI1BUF = 0x00; //Only one byte per command
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    trash = SPI1BUF;// clear SPI1 buffer using the "trash" variable
    
    SPI1BUF = data; //Write data. 
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    trash = SPI1BUF;// clear SPI1 buffer using the "trash" variable 
    
    SPI1BUF = 0x00; //Reading an extra byte here seems to be required for the MISO signal to work properly. 
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    trash = SPI1BUF;// clear SPI1 buffer using the "trash" variable
    
    SPI_CS_LAT = 1;
    
    return trash;
}

/*
 * spi_read_register
 * 
 * @Description
 *  reads an two eight bit data bytes to registers
 * 
 * @Params
 *  byte address - defines starting address for ADC
 */
uint8_t spi_read_reg(uint8_t address) {
    
    uint8_t value;
    
    uint8_t trash;
    
    SPI_CS_LAT = 0;
    //asm("nop");
    
    SPI1BUF = 0x20 | address;
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    trash = SPI1BUF;// clear SPI1 buffer using the "trash" variable
    
    SPI1BUF = 0x00; // only read 1 at a time
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    trash = SPI1BUF;// clear SPI1 buffer using the "trash" variable

    SPI1BUF = 0x00; //
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    value = SPI1BUF;// clear SPI1 buffer using the "trash" variable
    
    SPI1BUF = 0x00; //Reading an extra byte here seems to be required for the MISO signal to work properly. 
    while(!SPI1STATbits.SPIRBF); // wait for the SPI1 buffer full bit 
    trash = SPI1BUF;// clear SPI1 buffer using the "trash" variable
    
    SPI_CS_LAT = 1;
    
    return value;
}
